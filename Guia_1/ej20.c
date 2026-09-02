#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()
#include <unistd.h>     // fork() pipe() execlp() dup2() //close()
#include <sys/wait.h>   // wait()
#include <signal.h>     // Para las señales
#include <sys/socket.h> // sockets
#include <sys/types.h>  // socklen_t, pid_t, etc
#include <sys/un.h>     // UNIX Domain Sockets, sockaddr_un y AF_UNIX
#include <string.h>     // Para strcpy()
#include <stdbool.h>
#include <math.h>
/*
Escriba un programa para un servidor, que acepte varias conexiones de clientes, que reciba de cada
uno un número, y que vefique si es primo. La respuesta debe ser comunicada a cada cliente y luego
se debe terminar la conexión. Como los clientes nos podrían enviar números muy altos, el servidor
debería crear al menos 3 procesos hijos que distribuyan la carga de procesamiento entre ellos
*/

// gcc ej20.c -o ej20.out -lm && ./ej20.out

// Aca creo 1 hijo por proceso hasta un maximo, la alternativa que era la del ej creo, es crear
// n hijos fijos y que ellos hagan acept constantemente...bueeno..quedo asi

bool es_primo(int p){
    if(p<2){
        return false;
    }
    if (p == 2) return true;

    int max = floor(sqrt(p));
    for(int i = 2; i <= max+1; i++){
        if(p%i==0) return false;
    }
    return true;
}

void crear_servidor(int *my_server_socket, char *socket_name, int max_size) {
    struct sockaddr_un my_server_addr;
    uint slen = sizeof(my_server_addr);

    my_server_addr.sun_family = AF_UNIX;
    strcpy(my_server_addr.sun_path, socket_name);
    unlink(my_server_addr.sun_path);

    *my_server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (bind(*my_server_socket, (struct sockaddr *) &my_server_addr, slen) == -1) {
        perror("Error bind");
        exit(EXIT_FAILURE);
    }
    
    if (listen(*my_server_socket, max_size) == -1) {
        perror("Error listen");
        exit(EXIT_FAILURE);
    }
    printf("[Server]: Servidor %s creado (escuchando)...\n", socket_name);
}

void aceptar_cliente(int my_server_socket, int *client_socket) {
    struct sockaddr_un client_addr;
    uint clen = sizeof(client_addr);
    
    printf("[Servidor]: Esperando que alguien se conecte a mi servidor...\n");

    *client_socket = accept(my_server_socket, (struct sockaddr *) &client_addr, &clen);
    if (*client_socket == -1) {
        perror("Error accept");
        exit(EXIT_FAILURE);
    }
    printf("[Servidor]: ¡Cliente aceptado!\n");
}

void unirse_servidor(int i, char *socket_name, int *server_socket) {
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socket_name);

    printf("[P%d]: Me estoy conectando a %s...\n", i, socket_name);
    
    *server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    while (connect(*server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        // Si llega antes que el servidor
        usleep(50000);
    }
    printf("[P%d]: Conectado a %s exitosamente.\n", i, socket_name);
}


volatile sig_atomic_t current_childs = 0;
void handler(int sig){
    // Si varios hijos mueren juntos, en teoria podria solo llegar una llamada 
    // -1 es cualquiera y null es que no nos importa en que forma murio
    // WNOHANG es para que colgarnos si no queda ningun hijo mas muerto/zombie (da -1 cuadn no hay mas o que y que devuelve de normal?)
    // No hay que crear una mascara o algo asi para la señal?
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        current_childs--;
    }
}

void servidor(int max_size, int max_concurrent){
    int my_server_socket;
    crear_servidor(&my_server_socket, "unix_server", max_size);

    //signal(SIGCHLD, handler);
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Esto reinicia el accept() automáticamente
    // Sino cuando habia interrupcion el accept que estabamos esperando la quedaba
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Error sigaction");
        exit(EXIT_FAILURE);
    }

    // Crear los forks
    while (1) {
        if(max_concurrent==current_childs){
            printf("[SERVER] Todos mis hijos estan ocupados...espero\n");
            usleep(10000);
            continue;
        }

        int client_socket;

        aceptar_cliente(my_server_socket, &client_socket);
        
        current_childs++;
        if (fork() == 0) {
            // Cierra el socket del server porque su unico trabajo esta con el calcular primo
            close(my_server_socket); 
            
            int numero_recibido;
            read(client_socket, &numero_recibido, sizeof(int));
            
            bool resultado = es_primo(numero_recibido);
            
            write(client_socket, &resultado, sizeof(bool));
            
            close(client_socket);
            exit(EXIT_SUCCESS); 
        } 
        else {
            // El padre delego el client_socket al hijo, así que lo cierra de su lado
            close(client_socket); 
        }
    }
    close(my_server_socket);
}

void cliente(int i) {
    int server_socket;
    unirse_servidor(i, "unix_server", &server_socket);

    write(server_socket, &i, sizeof(int));

    bool respuesta;
    read(server_socket, &respuesta, sizeof(bool));

    if (respuesta) {
        printf("[Cliente %d]: El servidor dice que %d SI es primo.\n", i, i);
    } else {
        printf("[Cliente %d]: El servidor dice que %d NO es primo.\n", i, i);
    }
    close(server_socket);
    exit(EXIT_SUCCESS); 
}

int main(){
    int max_size = 50;      // La cola del server
    int max_concurrent = 25; // Cantidad de forks del server

    pid_t server_pid = fork();
    if(server_pid==0){
        servidor(max_size,max_concurrent);
    }

    int clientes = 80;
    int max_clients = 50; // Cantidad de procesos que van a pedir al servidor
    int alive_clients = 0;

    for(int i = 0; i < clientes; i++){
        if (alive_clients >= max_clients) {
            wait(NULL); 
            alive_clients--;
        }

        if(fork() == 0){
            cliente(i);
        }
        alive_clients++;
    }

    // Terminar de esperar a todos los que quedan
    while(alive_clients > 0){
        wait(NULL);
        alive_clients--;
    }


    printf("Todos los clientes terminaron. Apagando el servidor...\n");
    kill(server_pid, SIGTERM);
    wait(NULL);
    unlink("unix_server");
    return 0;
}