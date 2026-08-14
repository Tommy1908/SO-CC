#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("[JUAN] He nacido (PID: %d)\n", getpid());

    // 1. Juan crea a Julieta
    pid_t pid_julieta = fork();

    if (pid_julieta < 0) {
        perror("Error al crear a Julieta");
        return EXIT_FAILURE;
    }

    if (pid_julieta == 0) {
        // --- CÓDIGO DEL PROCESO JULIETA ---
        printf("  └── [JULIETA] He nacido (PID: %d, Padre: %d)\n", getpid(), getppid());

        // 2. Julieta crea a Jennifer
        pid_t pid_jennifer = fork();

        if (pid_jennifer < 0) {
            perror("Error al crear a Jennifer");
            exit(EXIT_FAILURE);
        }

        if (pid_jennifer == 0) {
            // --- CÓDIGO DEL PROCESO JENNIFER ---
            printf("       └── [JENNIFER] He nacido (PID: %d, Padre: %d)\n", getpid(), getppid());
            exit(EXIT_SUCCESS); // Jennifer termina su proceso
        }

        // Julieta espera a que Jennifer nazca y termine
        wait(NULL);
        exit(EXIT_SUCCESS); // Julieta termina su proceso
    }

    // --- CÓDIGO DEL PROCESO JUAN ---
    // Juan ESPERA obligatoriamente a que Julieta (y por tanto Jennifer) hayan nacido
    wait(NULL);

    // 3. Juan crea a Jorge LUEGO de que Jennifer ya nació
    pid_t pid_jorge = fork();

    if (pid_jorge < 0) {
        perror("Error al crear a Jorge");
        return EXIT_FAILURE;
    }

    if (pid_jorge == 0) {
        // --- CÓDIGO DEL PROCESO JORGE ---
        printf("  └── [JORGE] He nacido (PID: %d, Padre: %d)\n", getpid(), getppid());
        exit(EXIT_SUCCESS); // Jorge termina su proceso
    }

    // Juan espera a Jorge para recolectar su estado y terminar limpiamente
    wait(NULL);

    printf("[JUAN] Todos mis hijos y nietos han nacido en el orden correcto.\n");
    return EXIT_SUCCESS;
}