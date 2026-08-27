#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() //close()
#include <sys/wait.h> // wait()
#include <signal.h>    // Para las señales

enum {READ, WRITE};

int pipe_p_h1[2];
int pipe_h1_h2[2];
int pipe_h2_p[2];

void son1() {
    close(pipe_p_h1[WRITE]);
    close(pipe_h1_h2[READ]);
    close(pipe_h2_p[READ]);
    close(pipe_h2_p[WRITE]);

    int n;
    pid_t son2_pid;

    read(pipe_p_h1[READ], &son2_pid, sizeof(son2_pid));

    printf("[%d:HIJO1] Recibi el PID de mi hermano, es %d\n",getpid(),son2_pid);
    

    while(1){
        read(pipe_p_h1[READ], &n, sizeof(n));  // Recibe del Padre (0, 3, 6...)
        n++;
        printf("[%d:HIJO1] n=%d\n",getpid(),n);
        write(pipe_h1_h2[WRITE], &n, sizeof(n));// Le manda a Hijo 2 (1, 4, 7...)
    }
    exit(0);
}

void son2(pid_t son1_pid) {
    close(pipe_p_h1[WRITE]);
    close(pipe_p_h1[READ]);
    close(pipe_h1_h2[WRITE]);
    close(pipe_h2_p[READ]);

    int n;

    while(1){
        read(pipe_h1_h2[READ], &n, sizeof(n));   // Recibe de Hijo 1 (1, 4, 7...)
        n++;
        printf("[%d:HIJO2] n=%d\n",getpid(),n);
        write(pipe_h2_p[WRITE], &n, sizeof(n)); // Le manda al Padre (2, 5, 8...)
    }
    exit(0);
}

void father(pid_t son_pid1, pid_t son_pid2) {
    
    close(pipe_p_h1[READ]);
    close(pipe_h1_h2[WRITE]);
    close(pipe_h1_h2[READ]);
    close(pipe_h2_p[WRITE]);
    
    write(pipe_p_h1[WRITE], &son_pid2, sizeof(son_pid2));

    int n = 0;
    while(1){
        printf("[%d:PADRE] n=%d\n",getpid(),n);
        write(pipe_p_h1[WRITE], &n, sizeof(n));

        read(pipe_h2_p[READ], &n, sizeof(n));

        // La secuencia exacta da que 50 lo manda el Hijo 2 (50 % 3 = 2).
        if (n == 50) {
            break;
        }
        n++;
    }

    // Matamos a los procesos hijos y salimos
    kill(son_pid1, SIGKILL);
    kill(son_pid2, SIGKILL);

    wait(NULL);
    wait(NULL);

    close(pipe_p_h1[WRITE]);
    close(pipe_h2_p[READ]);

    exit(0);
}



int main(){

    if (pipe(pipe_p_h1) == -1) {
        perror("Error al crear el pipe");
        return 1;
    }
    if (pipe(pipe_h1_h2) == -1) {
        perror("Error al crear el pipe");
        return 1;
    }
    if (pipe(pipe_h2_p) == -1) {
        perror("Error al crear el pipe");
        return 1;
    }

    pid_t son_pid1 = fork();
    if(son_pid1 == 0){
        printf("Hijo 1 creado con exito\n");
        son1();
    }

    pid_t son_pid2 = fork();
    if(son_pid2 == 0){
        printf("Hijo 2 creado con exito\n");
        son2(son_pid1);
    }
    else {
        father(son_pid1, son_pid2);
    }

    return 0;
}



/*
Capacidad de buffer, el 11 especifica que bsend y breceive tienen capacidad cero (sincronicos). 
Un bsend bloquea hasta que el receptor invoca breceive. 
En UNIX, en cambio, tienen un buffer interno en memoria (parece que 64 KiB).
Entonces write() no se bloquea esperando a que el otro proceso lea, sino que retorna inmediatamente si hay espacio en el buffer.
Esta diferencia es la principal que hacia que se rompa al usar solo 2 pipes, habia chances que el hijo 2 lea y escriba y vuelva a leerse.
Ademas bsend/breceive eran con direccionamiento directo basado en PID, mientras que los pipes funcionan mediante descriptores de archivo heredados y anonimos en este caso.
*/
