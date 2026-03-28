/*
Supongamos que Juan tiene 2 hijos, Jorge y Julieta. A su vez Julieta tiene
una hija, Jennifer. Pero supongamos que luego de que naci´o Jennifer, Juan
tuvo a Jorge. Se requiere la creaci´on y ejecuci´on procesos que emulen la
vida de cada uno.
Pare el siguiente c´odigo...
¿En qu´e orden se imprimir´a en pantalla cada mensaje?
¿C´omo podr´ıa hacer para que se lancen los procesos en el momento
adecuado y sin problemas?
*/

# include <unistd.h>
# include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){

    printf("Soy Juan\n");

    pid_t fork_pid = fork();  //es el valor que dio el fork, para "juan" es el pid de "julieta", para julieta es 0.
    
    if(fork_pid == 0){
        printf("Soy Julieta\n");
        pid_t fork_pid = fork();
        if(fork_pid == 0){
            printf("Soy Jennifer\n");
            return 0;
        }
        waitpid(fork_pid, NULL, 0);
        return 0;
    }
    else{
        waitpid(fork_pid, NULL, 0);
        pid_t fork_pid = fork();
        if(fork_pid == 0){
            printf("Soy Jorge\n");
            return 0;
        }
    }
    
    return 0;
}