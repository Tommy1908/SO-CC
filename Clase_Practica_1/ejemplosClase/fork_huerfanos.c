#include <unistd.h>
#include <stdio.h>

typedef int pid_t;

int main()
{
    printf("Hola SO!\n");
    fork();
    printf("Aaaadios!\n");

    // Se va a correr 2 veces por el fork de arriba
    pid_t pidOrZero = fork();
    pid_t myPid = getpid();
    if (pidOrZero == 0)
    {
        sleep(2);
        printf("Soy el hijo porque me devolvio %d el PID, mi padre es ahora mismo: %d\n", pidOrZero, getppid());
        sleep(100);
    }
    else
    {
        printf("Soy el padre con PID: %d recien cree un hijo con id: %d\n", myPid, pidOrZero);
    }

    return 0;
}