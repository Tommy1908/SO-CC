#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al ejecutar fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // --- PROCESO HIJO ---
        printf("[HIJO]  Nací con PID: %d | Mi padre original es PPID: %d\n", getpid(), getppid());
        printf("[HIJO]  Esperando 2 segundos a que mi padre muera...\n");

        sleep(2); // Tiempo suficiente para que el padre ejecute exit()

        printf("\n[HIJO]  ¡He quedado huérfano!\n");
        printf("[HIJO]  Mi PID sigue siendo: %d, pero mi nuevo PPID es: %d\n", getpid(), getppid());
        printf("[HIJO]  Entrando en sleep(1000)... Usa la otra terminal para inspeccionarme.\n\n");

        sleep(1000);
        exit(EXIT_SUCCESS);
    } else {
        // --- PROCESO PADRE ---
        printf("[PADRE] Mi PID es: %d y he creado al hijo PID: %d\n", getpid(), pid);
        printf("[PADRE] Finalizando inmediatamente...\n\n");
        
        // El padre termina sin esperar al hijo
        exit(EXIT_SUCCESS);
    }
}