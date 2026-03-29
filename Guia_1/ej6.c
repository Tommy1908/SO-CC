#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void system_copy(const char *command)
{

    pid_t pid = fork();

    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
    }

    wait(NULL);
    return;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: \"ls -l\" \n");
        system_copy("ls -l");
    }
    else
    {
        printf("Uso: %s \n", argv[1]);
        system_copy(argv[1]);
    }

    return 0;
}