#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int variable = 10;

    printf("Valor inicial antes del fork: %d (Direccion: %p)\n\n", variable, (void*)&variable);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al ejecutar fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // --- CÓDIGO DEL HIJO ---
        variable = 99; // Modificamos la variable solo en el proceso hijo
        printf("[HIJO]  Modifique variable a: %d (Direccion virtual: %p)\n", variable, (void*)&variable);
        exit(EXIT_SUCCESS);
    } else {
        // --- CÓDIGO DEL PADRE ---
        wait(NULL); // Espera a que el hijo termine de modificar su variable

        printf("[PADRE] Mi variable sigue valiendo: %d (Direccion virtual: %p)\n", variable, (void*)&variable);
    }

    return EXIT_SUCCESS;
}