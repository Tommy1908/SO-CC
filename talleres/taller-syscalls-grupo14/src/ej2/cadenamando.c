/*
 * SSOO - La Cadena de Mando (template)
 *
 * Uso: ./bin/cadenamando N K J   (o: make run-ej2 N=4 K=5 J=1)
 *   N: cantidad de hijos a crear (1 <= N < 10)
 *   K: cantidad de rondas (K > 0)
 *   J: numero maldito (0 <= J < N)
 *
 * Formato de salida esperado (no lo cambies, la corrección automática
 * depende de esto):
 *
 *   HIJO <id> PID <pid> ULTIMAS_PALABRAS mando_total=<valor>
 *   SOBREVIVIENTE <id> PID <pid>
 *   PADRE mando_total=<valor>
 *
 * Restricciones:
 *   - Solo señales y llamadas de gestión de procesos (fork, la
 *     familia de wait, kill, pause, etc). Nada de pipes, memoria
 *     compartida ni sockets.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>

#define MAX_N 10

/* ---------------------------------------------------------------------
 * TODO 1: Declarar la(s) variable(s) global(es) que van a representar
 * la cuenta de pases del mando (una en el padre, y una por cada hijo,
 * ya que cada proceso tiene su propia copia desde el fork()).
 *
 * Pensá: ¿qué la va a leer? ¿qué la va a escribir? ¿desde dónde
 * (flujo normal del programa, o un manejador de señal)?
 *
 * ------------------------------------------------------------------ */

static int mando_total = 0;
volatile __sig_atomic_t señal = 0;
sigset_t mascara_bloqueada, mascara_original;

/* TODO 2: variables globales que necesite el padre para llevar el
 * estado del juego: PIDs de los hijos, quién sigue vivo, a quién le
 * toca el mando ahora, cuántas rondas van, etc.
 */
static pid_t pids[MAX_N];

static int N, K, J;

/* ---------------------------------------------------------------------
 * Manejadores de señal.
 *
 * Recordá: dentro de un manejador sólo podés usar funciones
 * async-signal-safe (nada de printf). Si necesitás "avisar" algo al
 * resto del programa, la forma habitual es levantar una bandera y
 * procesarla fuera del handler.
 * ------------------------------------------------------------------ */

static void manejador_mando_recibido(int sig)
{
    /* TODO 3 (hijo): marcar que llegó el mando (sólo levantar un
     * flag acá; el trabajo real -sortear el número, decidir si
     * termina o sigue- se hace en el loop principal del hijo). */
    señal = sig;
}

static void manejador_alguien_paso(int sig)
{
    (void)sig;
    /* TODO 4 (padre): un hijo avisó que quiere pasar el mando.
     * ¿Qué es lo mínimo que hay que hacer acá adentro, y qué es mejor
     * dejar para el loop principal del padre? */
    señal = sig;
}

static void manejador_hijo_termino(int sig)
{
    (void)sig;
    /* TODO 5 (padre): un hijo terminó y el padre fue notificado por señal.
     * Ojo: esta señal no encola. Si reapeás con un único wait, ¿qué puede pasar
     * si mueren dos hijos casi al mismo tiempo (por ejemplo, cuando el
     * padre manda SIGKILL a varios sobrevivientes juntos al final)?
     * ¿Hace falta hacer el reapeo real acá adentro, o alcanza con que
     * este handler haga que pause() se despierte? */
    señal = sig;
}

int _buscar_siguiente_flag(int c, bool inclusive)
{
    for (int i = c + (!inclusive ? 1 : 0); i < N; i++)
    {
        if (pids[i] != -1)
            return i;
    }
    return -1;
}

int buscar_siguiente(int c) { return _buscar_siguiente_flag(c, false); }
int buscar_primero() { return _buscar_siguiente_flag(0, true); }

/* ---------------------------------------------------------------------
 * Lógica del hijo
 * ------------------------------------------------------------------ */

static void correr_hijo(int id)
{
    /* TODO 6: sembrar el generador de números aleatorios. Pensá bien
     * en qué momento hay que hacer esto (antes o después del fork?) y
     * con qué semilla, para que cada hijo saque números distintos. */

    srand(id);

    /* TODO 7: instalar el/los manejador(es) de señal que necesite este
     * proceso hijo (con signal(), no con sigaction). */

    signal(SIGUSR1, manejador_mando_recibido);

    for (;;)
    {
        sigsuspend(&mascara_original);
        if (señal != SIGUSR1)
            continue;
        /* TODO 8: si llegó el mando (ver bandera de TODO 3):
         *   - incrementar la copia local de la cuenta de pases
         *   - sortear un numero entre 0 y N-1
         *   - si es el numero maldito J: imprimir las ultimas palabras
         *     con el formato pedido y terminar (exit) usando el propio
         *     id como codigo de salida
         *   - si no: avisarle al padre que hay que pasar el mando
         *     (¿con qué señal? ¿a quién hay que mandársela?), y volver
         *     a esperar
         */
        señal = 0;
        mando_total += 1;
        int j = rand() % N;
        // printf("HIJO %d PID %d SAQUE %d\n", id, getpid(), j);
        if (j == J)
        {
            printf("HIJO %d PID %d ULTIMAS_PALABRAS mando_total=%d\n", id, getpid(), mando_total);
            exit(id);
        }
        kill(getppid(), SIGUSR1);
    }
}

/* ---------------------------------------------------------------------
 * Lógica del padre
 * ------------------------------------------------------------------ */

static int validar_parametros(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "uso: %s N K J\n", argv[0]);
        return -1;
    }
    N = atoi(argv[1]);
    K = atoi(argv[2]);
    J = atoi(argv[3]);
    if (N <= 0 || N >= MAX_N)
    {
        fprintf(stderr, "N debe ser mayor a 0 y menor a %d\n", MAX_N);
        return -1;
    }
    if (K <= 0)
    {
        fprintf(stderr, "K debe ser mayor a 0\n");
        return -1;
    }
    if (J < 0 || J >= N)
    {
        fprintf(stderr, "J debe cumplir 0 <= J < N\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (validar_parametros(argc, argv) != 0)
    {
        return 1;
    }

    /* TODO 9: instalar en el padre los manejadores de señal que
     * necesite (¿cuáles señales le van a llegar al padre a lo largo
     * del juego?). */
    sigemptyset(&mascara_bloqueada);
    sigaddset(&mascara_bloqueada, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mascara_bloqueada, &mascara_original);

    for (int i = 0; i < N; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            /* TODO: decidir qué hacer si falla un fork a mitad de
             * camino (¿matar a los ya creados?) */
            for (int j = 0; j < i; j++)
            {
                kill(pids[j], SIGKILL);
                waitpid(pids[j], NULL, 0);
            }

            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            correr_hijo(i);
            _exit(1); /* correr_hijo nunca deberia retornar */
        }
        pids[i] = pid;
    }
    for (int i = N; i < MAX_N; i++)
    {
        pids[i] = -1;
    }

    signal(SIGUSR1, manejador_alguien_paso);
    signal(SIGCHLD, manejador_hijo_termino);

    /* TODO 10: arrancar el juego (¿quién tiene el mando al empezar la
     * primera ronda?), y despues loopear esperando señales (pause())
     * hasta cubrir las K rondas o quedar un solo sobreviviente.
     *
     * En cada despertar conviene, sin asumir nada sobre cuántas
     * señales concretas llegaron, revisar el estado real de los
     * hijos (pista: la familia de wait con WNOHANG en loop). */

    for (int r = 0; r < K; r++)
    {
        int mando = buscar_primero();
        while (mando != -1)
        {
            // printf("PADRE MANDANDO A %d\n", mando);
            kill(pids[mando], SIGUSR1);
            while (!señal)
                sigsuspend(&mascara_original);
            if (señal == SIGUSR1)
            {
                mando_total += 1;
            }
            if (señal == SIGCHLD)
            {
                waitpid(pids[mando], NULL, 0);
                pids[mando] = -1;
            }
            señal = 0;
            mando = buscar_siguiente(mando);
        }
    }

    /* TODO 11: al terminar, imprimir los sobrevivientes (formato
     * pedido), mandarles SIGKILL a los que queden vivos, reapearlos
     * (¡que no queden zombies!), e imprimir la cuenta final del
     * padre. */

    int s = -1;
    while ((s = buscar_siguiente(s)) != -1)
    {
        printf("SOBREVIVIENTE %d PID %d\n", s, pids[s]);
        fflush(stdout);
        kill(pids[s], SIGKILL);
        waitpid(pids[s], NULL, 0);
    }
    printf("PADRE mando_total=%d\n", mando_total);
    fflush(stdout);
    return 0;
}
