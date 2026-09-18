/*
 * SSOO - Relay espejo (template)
 *
 * Ejercicio 1, Parte B: replicar exactamente el comportamiento del
 * binario de la cátedra bin/relay.
 *
 * Uso: ./bin/relay_espejo N   (o: make run-ej1 N=3)
 *   N: cantidad de hijos a lanzar (1 <= N <= 5)
 *
 * La corrección automática compara tu salida contra la de 'relay'
 * línea por línea, normalizando los PIDs (que obviamente cambian). Los
 * printf() que ya están escritos abajo son parte de esa salida
 * esperada: no los toques. Lo que falta es la sincronización del
 * medio. Para comparar a mano:
 *
 *   make run-relay N=3    / make run-ej1 N=3      (salida)
 *   make strace-relay N=3 / make strace-ej1 N=3   (syscalls)
 *
 * Restricciones:
 *   - Toda la sincronización se resuelve con fork(), signal(), kill(),
 *     pause() y wait(). Los tests verifican en la traza de strace que
 *     esas syscalls aparezcan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

volatile int signal_received = 0;

/* TODO: Implementar el handler de señal.
 *
 * Esta función debe ser invocada cuando el hijo reciba la señal que
 * identificaste con strace en la Parte A.
 *
 * IMPORTANTE: Usar write() en lugar de printf() dentro de handlers.
 */
void handler(int sig)
{
    (void)sig;
    signal_received = 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]);
    if (N < 1 || N > 5)
    {
        fprintf(stderr, "N debe estar entre 1 y 5\n");
        exit(1);
    }

    pid_t *pids = malloc(N * sizeof(pid_t));

    printf("[PADRE %d] Lanzando %d procesos\n", getpid(), N);
    fflush(stdout);

    for (int i = 0; i < N; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            /* Proceso hijo */
            signal(SIGUSR1, handler);
            printf("[HIJO %d] Esperando señal...\n", getpid());
            fflush(stdout);
            while (!signal_received)
                pause();
            printf(". [HIJO %d] Recibió SIGUSR1\n", getpid());
            fflush(stdout);
            exit(0);
        }
        else if (pid > 0)
        {
            /* Proceso padre */
            pids[i] = pid;
        }
        else
        {
            perror("fork");
            exit(1);
        }
    }

    /* TODO: El padre debe esperar 1 segundo */
    sleep(1);

    printf("[PADRE %d] Enviando señales a los hijos\n", getpid());
    fflush(stdout);

    /* TODO: El padre envía SIGUSR1 a cada hijo */
    /* Usar un usleep(100000) entre cada kill para simular delay */
    for (int i = 0; i < N; i++)
    {
        kill(pids[i], SIGUSR1);
        usleep(100000);
    }

    printf("[PADRE %d] Esperando a que terminen los hijos\n", getpid());
    fflush(stdout);

    /* TODO: El padre debe gestionar la sincronización con cada hijo */
    for (int i = 0; i < N; i++)
        wait(NULL);

    printf("[PADRE %d] Todos los hijos terminaron\n", getpid());
    fflush(stdout);

    free(pids);
    return 0;
}
