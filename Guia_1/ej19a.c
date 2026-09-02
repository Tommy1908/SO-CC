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
//  Proceso1 envía a Proceso2 el valor 2
//  Proceso2 envía a Proceso1 el valor 3

//Los voy a hacer hermanos, pero podrian perfectamente ser 2 codigos separados...es para que quede todo aca

// Va a crear el servidor
void proceso_1(){
    int server_socket;
    struct sockaddr_un server_addr;
    uint slen = sizeof(server_addr);

    int client_socket;
    struct sockaddr_un client_addr;
    uint clen = sizeof(client_addr);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "proceso_1_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(server_socket, (struct sockaddr *) &server_addr, slen) == -1) {
        perror("Error bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 1) == -1) {
        perror("Error listen");
        exit(EXIT_FAILURE);
    }
    printf("[P1-servidor]: esperando conexión del cliente...\n");
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
    if (client_socket == -1) {
        perror("Error accept");
        exit(EXIT_FAILURE);
    }
    int value = 0;
    while(1){
        if (write(client_socket, &value, sizeof(value)) == -1) {
            perror("Error al escribir");
            exit(EXIT_FAILURE);
        }
        printf("[P1] Envio %d\n",value);

        if (read(client_socket, &value, sizeof(value)) == 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
        printf("[P1] Recibo %d\n",value);

        if(value ==3){
            close(client_socket);
            break;
        }
        value++;
    }

    exit(EXIT_SUCCESS);
}

// Va ser cliente
void proceso_2(){
    int server_socket;
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "proceso_1_socket");

    printf("[P2]: me estoy conectando con el servidor...\n");
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    while (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("[P2]: Error al conectarse...reintentando\n");
        usleep(10000);
    }
    printf("[P2]: Conectado\n");

    //if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    //    perror("Error al conectar al servidor");
    //    exit(EXIT_FAILURE);
    //}

    int value;
    while(1){

        int bytes_leidos = read(server_socket, &value, sizeof(value));
        if (bytes_leidos == -1) {
            perror("Error en la lectura");
            exit(EXIT_FAILURE);
        } 
        else if (bytes_leidos == 0) {
            printf("[P2] EOF recibido, servidor cerro la conexion.\n");
            break; 
        }
        printf("[P2] Recibo %d\n",value);

        value++;
        if (write(server_socket, &value, sizeof(value)) == -1) {
            perror("Error al escribir");
            exit(EXIT_FAILURE);
        }
        printf("[P2] Envio %d\n",value);
    }
    close(server_socket);

    exit(EXIT_SUCCESS);
}

int main(){

    if(fork()==0){
        proceso_1();
    }
    if(fork()==0){
        proceso_2();
    }
    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);
}