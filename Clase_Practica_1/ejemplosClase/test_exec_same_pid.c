#include <unistd.h>
#include <stdio.h>

int main()
{

    printf("Mi pid:%d\n", getpid());

    // 1. Definimos los argumentos
    char *args[] = {"./pid", NULL};

    // 2. Definimos el entorno (vacío en este caso)
    char *env[] = {NULL};

    // 3. Llamamos a execve
    execve("./pid", args, env);

    return 0;
}
