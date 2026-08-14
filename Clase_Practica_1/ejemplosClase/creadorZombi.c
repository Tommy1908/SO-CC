#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al hacer fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Código del HIJO: termina de inmediato
        printf("[HIJO] PID: %d -> Finalizando ejecucion...\n", getpid());
        exit(0); 
    } else {
        // Código del PADRE: duerme sin llamar a wait()
        printf("[PADRE] PID: %d -> Hijo creado con PID: %d\n", getpid(), pid);
        printf("[PADRE] Durmiendo 30 segundos. Inspecciona la terminal ahora...\n");
        sleep(30);
        printf("[PADRE] Finalizado. Al morir el padre, el zombi se limpia.\n");
    }

    return EXIT_SUCCESS;
}