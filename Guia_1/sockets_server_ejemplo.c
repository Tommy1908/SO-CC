#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()
#include <unistd.h>     // fork() pipe() execlp() dup2() //close()
#include <sys/wait.h>   // wait()
#include <signal.h>     // Para las señales
#include <sys/socket.h> // sockets
#include <sys/types.h>  // socklen_t, pid_t, etc
#include <sys/un.h>     // UNIX Domain Sockets, sockaddr_un y AF_UNIX
#include <string.h>     // Para strcpy()

int main(){
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    socklen_t slen = sizeof(server_addr);
    socklen_t clen = sizeof(client_addr);
    // sockaddr_un -> sun_family: El tipo de familia de direcciones
    //             -> sun_path: La ruta del archivo en el disco que representará este socket
    // socklen_t -> uint


    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    // El cast se hace porque bind usa un generico sockaddr para el resto de tipos
    // Aca se crea el archivo unix_socket, si alguien le escribe viene a nosotros
    if (bind(server_socket, (struct sockaddr *) &server_addr, slen) == -1) { 
        perror("Error");
        exit(EXIT_FAILURE);
    }
    // La cola de conexiones pendientes es 1 en este caso. Osea si estamos ocupado con
    // 1 cliente (podremos evitarlo luego), dejamos esperando a 1 mas, luego de eso, otro recibiria un error
    if (listen(server_socket, 1) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }   
    printf("servidor: esperando conexión del cliente...\n");
    
    while(1) {
        // Nos bloqueamos hasta que un cliente se conecte
        // accept() crea un socket nuevo. 
        //El resultado se guarda en client_socket. El server_socket original solo sirve de "recepcionista". client_socket es una "via privada" para hablar con ese cliente en particular.
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        if (client_socket == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        int id;
        if (read(client_socket, &id, sizeof(id)) == 0) { // EOF -> signficia que la conexion se corto 
            perror("Error");
            exit(EXIT_FAILURE);
        }

        char msg[5];
        if (read(client_socket, &msg, 5) == 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        printf("servidor: recibí %s del cliente #%d\n", msg, id);

        // Compilar con el sleep y luego ver qué pasa si matamos al cliente
        // antes de que el servidor responda.
        printf("servidor: me voy a dormir un rato...\n");
        sleep(100);

        char pong[5] = "pong\0";
        if (write(client_socket, &pong, 5) == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        close(client_socket);
    }

    exit(EXIT_SUCCESS);
}