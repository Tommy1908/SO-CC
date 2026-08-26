#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()


enum { READ, WRITE };

int pipes[2];

// Debe ejecutar "ls -al"
void ejecutar_hijo_1() {
    dup2(pipes[WRITE], STDOUT_FILENO);

    close(pipes[READ]);
    close(pipes[WRITE]); // Sin este no va a andar el wc!

    execlp("ls", "ls", "-al", NULL);
    exit(1); //error
}

// Debe ejecutar "wc -l"
void ejecutar_hijo_2() {
    dup2(pipes[READ], STDIN_FILENO);

    close(pipes[READ]);
    close(pipes[WRITE]); // Sin este no va a andar el wc!

    execlp("wc", "wc", "-l", NULL);
    exit(1); //error
}

int main(int argc, char const* argv[]) {    
    
    if (pipe(pipes) == -1) {
        perror("Error al crear el pipe");
        return 1;
    }
    
    pid_t pid_ls = fork();
    if(pid_ls==0) ejecutar_hijo_1();

    pid_t pid_wc = fork();
    if(pid_wc==0) ejecutar_hijo_2();

    close(pipes[READ]);
    close(pipes[WRITE]); // Sin este no va a andar el wc!

    wait(NULL);
    wait(NULL);

    return 0;
}