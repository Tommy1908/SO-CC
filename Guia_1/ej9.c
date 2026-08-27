#include <stdio.h>     // Para printf
#include <stdlib.h>    // Para exit, EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>    // Para fork
#include <sys/types.h> // Para pid_t
#include <signal.h>    // Para las señales
#include <sys/wait.h>  // Para wait

void handler(int sig){
    //No necesitamos comportamiento, solo que resuma donde lo dejamos
}

void comportamiento_hijo(sigset_t *mascara_original){
    for(int i = 0; i < 3; i++){
        sigsuspend(mascara_original); // Esperar al padre, a que mande su ping
        printf("PONG %d\n", getpid());
        kill(getppid(), SIGUSR1);
    }
}

void comportamiento_padre(pid_t son_pid, sigset_t *mascara_original){
    //sleep(1); // Para testear que el orden es correcto y no por el scheduler
    for(int i = 0; i < 3; i++){
        printf("PING %d\n", getpid());
        kill(son_pid, SIGUSR1);
        sigsuspend(mascara_original); 
    }
}

int main(){

    struct sigaction sa = { .sa_handler = handler };
    sigaction(SIGUSR1, &sa, NULL);

    sigset_t mascara_bloqueada, mascara_original;
    sigemptyset(&mascara_bloqueada);
    sigaddset(&mascara_bloqueada, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mascara_bloqueada, &mascara_original); // Desde aca, SIGUSR1 queda trabado, y no perdemos ninguna señal


    pid_t son_pid = fork();
    if(son_pid < 0){
        perror("Error al ejecutar fork");
        return EXIT_FAILURE;
    }

    if(son_pid != 0){
        // Padre
        char opcion;
        do{
            comportamiento_padre(son_pid, &mascara_original);
            printf("\nTerminar ejecucion (1/0):");
            scanf(" %c", &opcion);
            if(opcion == '1'){
                kill(son_pid, SIGTERM);
                wait(0); // Esperar a que muera
                exit(EXIT_SUCCESS);
            }
        }while(opcion != '1');
        
    }
    else{
        // Hijo
        while(1){
            comportamiento_hijo(&mascara_original);
        }

    }
}