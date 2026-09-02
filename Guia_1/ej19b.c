#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()
#include <unistd.h>     // fork() pipe() execlp() dup2() //close()
#include <sys/wait.h>   // wait()
#include <signal.h>     // Para las señales
#include <sys/socket.h> // sockets
#include <sys/types.h>  // socklen_t, pid_t, etc
#include <sys/un.h>     // UNIX Domain Sockets, sockaddr_un y AF_UNIX
#include <string.h>     // Para strcpy()

// Escribir un programa entre dos procesos, no necesariamente emparentados, que efectúen la siguiente secuencia de mensajes entre ambos:
//  Proceso1 envía a Proceso2 el valor 0
//  Proceso2 envía a Proceso1 el valor 1
//  Proceso3 envía a Proceso1 el valor 2
//  Proceso1 envía a Proceso2 el valor 3
// Hasta el valor 50

//Los voy a hacer hermanos, pero podrian perfectamente ser 2 codigos separados...es para que quede todo aca

// Cada proceso va a crear un servidor y unirse a un servidor
// P1 crea p_1_socket y se une a p_3_socket
// P2 crea p_2_socket y se une a p_1_socket
// P3 crea p_3_socket y se une a p_2_socket

/*
EL orden crear, unirse, aceptar no te bloquea porque el connect no espera al accept.
El listen crea como una sala de hueco 1 en este caso, al que no unimos y printea que nos unimos exitosamente
(todavia no te aceptaron realmente, pero vos podrias incluso escribir creo), pero al salir de eso, podemos nosotros ir a aceptar, y el otro ira a aceptarnos realmente
El while connect, no espera a que nos accepten, sino a que la sala de espera este disponible
*/

void crear_servidor(int i, int *my_server_socket, char *socket_name) {
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
    
    if (listen(*my_server_socket, 1) == -1) {
        perror("Error listen");
        exit(EXIT_FAILURE);
    }
    printf("[P%d]: Servidor %s creado (escuchando)...\n", i, socket_name);
}

void aceptar_cliente(int i, int my_server_socket, int *client_socket) {
    struct sockaddr_un client_addr;
    uint clen = sizeof(client_addr);
    
    printf("[P%d]: Esperando que alguien se conecte a mi servidor...\n", i);
    *client_socket = accept(my_server_socket, (struct sockaddr *) &client_addr, &clen);
    if (*client_socket == -1) {
        perror("Error accept");
        exit(EXIT_FAILURE);
    }
    printf("[P%d]: ¡Cliente aceptado!\n", i);
}

void unirse_servidor(int i, char *socket_name, int *server_socket) {
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socket_name);

    printf("[P%d]: Me estoy conectando a %s...\n", i, socket_name);
    
    *server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    while (connect(*server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        usleep(50000);
    }
    printf("[P%d]: Conectado a %s exitosamente.\n", i, socket_name);
}


void proceso_1() {
    int my_server_socket, client_socket_p2, server_socket_p3;
    
    crear_servidor(1, &my_server_socket, "proceso_1_socket");
    unirse_servidor(1, "proceso_3_socket", &server_socket_p3);
    aceptar_cliente(1, my_server_socket, &client_socket_p2);

    int value = 0;
    while(value<50){
        if (write(client_socket_p2, &value, sizeof(value)) == -1) {perror("Error al escribir"); exit(EXIT_FAILURE);}
        printf("[P1] Envio %d\n",value);

        int bytes_leidos = read(server_socket_p3, &value, sizeof(value));
        if (bytes_leidos == -1) {perror("Error en la lectura"); exit(EXIT_FAILURE);} 
        else if (bytes_leidos == 0) {printf("[P1] EOF recibido, servidor cerro la conexion.\n");break;}
        else {printf("[P1] Recibo %d\n",value);}

        value++;
    }

    // Limpiar y salir
    close(client_socket_p2);
    close(server_socket_p3);
    close(my_server_socket);
    exit(EXIT_SUCCESS);
}

void proceso_2() {
    int my_server_socket, client_socket_p3, server_socket_p1;
    
    crear_servidor(2, &my_server_socket, "proceso_2_socket");
    //sleep(5);
    unirse_servidor(2, "proceso_1_socket", &server_socket_p1);
    aceptar_cliente(2, my_server_socket, &client_socket_p3);

    int value=0;
    while(value<50){
        int bytes_leidos = read(server_socket_p1, &value, sizeof(value));
        if (bytes_leidos == -1) {perror("Error en la lectura"); exit(EXIT_FAILURE);} 
        else if (bytes_leidos == 0) {printf("[P2] EOF recibido, servidor cerro la conexion.\n");break;}
        else {printf("[P2] Recibo %d\n",value);}

        value++;

        if (write(client_socket_p3, &value, sizeof(value)) == -1) {perror("Error al escribir"); exit(EXIT_FAILURE);}
        printf("[P2] Envio %d\n",value);
    }

    // Limpieza
    close(client_socket_p3);
    close(server_socket_p1);
    close(my_server_socket);
    exit(EXIT_SUCCESS);
}

void proceso_3() {
    int my_server_socket, client_socket_p1, server_socket_p2;
    
    crear_servidor(3, &my_server_socket, "proceso_3_socket");
    //sleep(5);
    unirse_servidor(3, "proceso_2_socket", &server_socket_p2);
    aceptar_cliente(3, my_server_socket, &client_socket_p1);

    int value=0;
    while(value<50){
        int bytes_leidos = read(server_socket_p2, &value, sizeof(value));
        if (bytes_leidos == -1) {perror("Error en la lectura"); exit(EXIT_FAILURE);} 
        else if (bytes_leidos == 0) {printf("[P3] EOF recibido, servidor cerro la conexion.\n");break;}
        else {printf("[P3] Recibo %d\n",value);}

        value++;

        if (write(client_socket_p1, &value, sizeof(value)) == -1) {perror("Error al escribir"); exit(EXIT_FAILURE);}
        printf("[P3] Envio %d\n",value);
    }

    close(client_socket_p1);
    close(server_socket_p2);
    close(my_server_socket);
    exit(EXIT_SUCCESS);
}

int main(){

    if(fork()==0){
        proceso_1();
    }
    if(fork()==0){
        proceso_2();
    }
    if(fork()==0){
        proceso_3();
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);
}