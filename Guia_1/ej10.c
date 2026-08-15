#include <stdio.h>     // Para printf
#include <stdlib.h>    // Para exit, EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>    // Para fork
#include <sys/types.h> // Para pid_t
#include <signal.h>    // Para las señales
#include <sys/wait.h>  // Para wait



int main(){

    pid_t son_pid = fork();
    if(son_pid != 0){
        printf("Soy Juan \n");
        sleep(1);
        waitpid(-1, NULL, 0);
        pid_t son_pid = fork();
        if(son_pid==0){
            printf("Soy Jorge \n");
            sleep(1);
        }

    }
    else{
        printf("Soy Julieta\n");
        son_pid = fork();
        if(son_pid == 0){
            printf("Soy Jennifer\n");
            sleep(1);
        } 
    }
}