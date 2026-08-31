#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Necesario para wait()
#include "dado.h"

enum
{
    READ,
    WRITE
};
enum
{
    LESTER,
    ELIZA
};

// 1. Mover las variables globales arriba para que las funciones las vean
int pipesEliza[2];
int pipesLester[2];

void lester()
{
    // 2. Buena práctica: cerrar TODO lo que no voy a usar ANTES de hacer la lógica
    close(pipesEliza[READ]);
    close(pipesEliza[WRITE]);
    close(pipesLester[READ]);

    int dado_lester = tirar_dado();
    write(pipesLester[WRITE], &dado_lester, sizeof(dado_lester));

    close(pipesLester[WRITE]); // Cierro antes de salir

    // 3. exit requiere un argumento (0 es éxito)
    exit(0);
}

void eliza()
{
    // Hago lo mismo que en Lester: cierro lo que no me sirve
    close(pipesLester[READ]);
    close(pipesLester[WRITE]);
    close(pipesEliza[READ]);

    int dado_eliza = tirar_dado();
    write(pipesEliza[WRITE], &dado_eliza, sizeof(dado_eliza));

    close(pipesEliza[WRITE]);
    exit(0);
}

int main(int argc, char const *argv[])
{
    // Creo los pipes ANTES de hacer los forks
    pipe(pipesEliza);
    pipe(pipesLester);

    int pidLester = fork();
    if (pidLester == 0)
    {
        lester();
    }

    int pidEliza = fork();
    if (pidEliza == 0)
    {
        eliza();
    }

    // 4. CRÍTICO: El padre no va a escribir, debe cerrar los extremos de escritura
    close(pipesLester[WRITE]);
    close(pipesEliza[WRITE]);

    int resultado_lester;
    // Ahora si Lester falla, este read devolverá 0 (EOF) en lugar de bloquearse
    read(pipesLester[READ], &resultado_lester, sizeof(resultado_lester));

    int resultado_eliza;
    read(pipesEliza[READ], &resultado_eliza, sizeof(resultado_eliza));

    // Cierro los extremos de lectura ya que terminé de usarlos
    close(pipesLester[READ]);
    close(pipesEliza[READ]);

    // 5. Recojo a los hijos para que no queden zombies
    wait(NULL);
    wait(NULL);

    if (resultado_eliza > resultado_lester)
    {
        printf("Gano Eliza\n");
    }
    else if (resultado_eliza < resultado_lester)
    {
        printf("Gano Lester\n");
    }
    else
    {
        printf("Empate\n");
    }

    return 0;
}