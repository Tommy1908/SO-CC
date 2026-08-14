#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void manejador_sigint(int sig) {
    const char mensaje[] = "\n[MANEJADOR] ¡Señal SIGINT capturada con signal()!\n";
    write(STDOUT_FILENO, mensaje, sizeof(mensaje) - 1);
}

int main(void) {

    /* =========================================================================
     * OPCIÓN TRADICIONAL CON signal() (1 sola línea)
     * =========================================================================
     */
    if (signal(SIGINT, manejador_sigint) == SIG_ERR) {
        perror("Error al registrar la señal con signal");
        return EXIT_FAILURE;
    }

    /*
     * ALTERNATIVA con sigaction():
     *
     * struct sigaction sa;
     * sa.sa_handler = manejador_sigint;
     * sigemptyset(&sa.sa_mask);
     * sa.sa_flags = 0;
     * sigaction(SIGINT, &sa, NULL);
     */

    printf("Programa iniciado (PID: %d).\n", getpid());
    printf("Presiona Ctrl+C en la terminal...\n\n");

    while (1) {
        printf("Ejecutando tareas de fondo...\n");
        sleep(2);
    }

    return EXIT_SUCCESS;
}