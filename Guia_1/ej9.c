#include <stdio.h>     // Para printf
#include <stdlib.h>    // Para exit, EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>    // Para fork
#include <sys/types.h> // Para pid_t
#include <signal.h>    // Para las señales

int main(){

    pid_t son_pid = fork();
    if(son_pid < 0){
        perror("Error al ejecutar fork");
        return EXIT_FAILURE;
    }

    if(son_pid != 0){
        // Padre
        for(int i = 0; i < 3; i++){
            printf("PING %d\n", getpid());
            pause();
        }
    }
    else{
        // Hijo
        for(int i = 0; i < 3; i++){
            printf("PONG %d\n", getpid());
            kill(getppid(), SIGUSR1);
        }
    }
}