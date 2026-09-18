<style>
  @page { size: A4 landscape; margin: 0.8cm; }
  body {
    column-count: 3; column-gap: 15px; column-rule: 1px solid #ccc;
    font-family: "Helvetica Neue", Arial, sans-serif;
    font-size: 9px; line-height: 1.1; text-align: justify; margin: 0;
  }
  h2 { font-size: 10px; margin: 4px 0 2px; border-bottom: 1px solid #666; padding-bottom: 1px; }
  h3 { font-size: 9px; margin: 3px 0 1px; color: #333; }
  
  p, ul { margin: 1px 0 2px 0; padding-left: 12px; }
  li { margin-bottom: 1px; }
  pre { 
    background-color: #f4f4f4; border: 1px solid #ddd; border-radius: 0px; 
    padding: 2px 4px; margin: 2px 0; font-size: 8px; line-height: 1.05; 
    white-space: pre-wrap; word-wrap: break-word; 
    break-inside: avoid;
    page-break-inside: avoid;
  }
  code { font-family: "Consolas", monospace; background: #f4f4f4; padding: 0 1px; }
  pre code { background: transparent; padding: 0; }
  table { width: 100%; border-collapse: collapse; font-size: 8px; margin: 2px 0; }
  th, td { padding: 1px 2px; border: 1px solid #999; }
</style>


### `wait()` / `waitpid()` — esperar a que terminen los hijos

```c
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

for (int i = 0; i < p->count; i++)
    wait(NULL);

static void reap_children(int sig) {
    int saved = errno;
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved;
}
```

***
### Familia `exec` — reemplazar la imagen del proceso

```c
int execvp(const char *file, char *const argv[]);
int execlp(const char *file, const char *arg, ... /* (char *)NULL */);

// argv ya viene terminado en NULL, como lo arma parser.c
execvp(argv[0], argv);          // ejemplo: argv = {"wc", "-l", NULL}

// variante variádica (execlp), cada argumento suelto y terminado en NULL
execlp("ls", "ls", "-al", NULL);
```

---


```c
int pipe(int pipefd[2]);

// minishell.c — un pipe por cada '|' del pipeline
int (*pipes)[2] = calloc(p->count - 1, sizeof(int[2]));
for (int i = 0; i < p->count - 1; i++)
    pipe(pipes[i]);
```

### `dup2()` — redirigir un descriptor

```c
int dup2(int oldfd, int newfd);
//Hace que `newfd` apunte a lo mismo que `oldfd` (cerrando antes lo que `newfd` tuviera). Es la forma estándar de decir "mi stdout ahora es este pipe".

// El comando i-ésimo de un pipeline: de qué pipe lee, a cuál escribe
if (i != 0)         dup2(pipes[i - 1][READ],  STDIN_FILENO);
if (i != count - 1) dup2(pipes[i][WRITE], STDOUT_FILENO);
```


### `read()` / `write()` — leer y escribir

```c
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

Devuelven la cantidad de bytes efectivamente leídos/escritos (puede ser **menos** de lo pedido: hay que insistir en un loop). `read()` devuelve `0` cuando el otro extremo cerró (EOF).

```c
// anillo.c — pasar un entero de un hijo al siguiente por el anillo
int msg;
int n = read(read_pipe, &msg, sizeof(int));
if (n == 0) break;              // el anterior cerró su extremo: se acabó
msg++;
write(write_pipe, &msg, sizeof(int));
```

***
## 3. Sockets 
`socket()`+`bind()`+`listen()`+`accept()` van siempre del lado servidor; `socket()`+`connect()` del lado cliente; `send()`/`recv()` de ambos lados.
### Lado servidor

```c
int socket(int domain, int type, int protocol);
```

Crea el socket. `domain`: `AF_UNIX`

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

Le asigna una dirección al socket (para `AF_UNIX`, una ruta del sistema de archivos).

```c
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

`listen()` pone el socket en modo pasivo con una cola de `backlog` conexiones entrantes. `accept()` saca una conexión de esa cola y devuelve un **descriptor nuevo** para hablar con ese cliente en particular (el socket original sigue siendo el de "escuchar", nunca se usa para datos).

```c
// servidor.c — socket UNIX: crear, bindear, escuchar
struct sockaddr_un addr;
int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
memset(&addr, 0, sizeof(addr));
addr.sun_family = AF_UNIX;
unlink(path);
strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
listen(listen_fd, BACKLOG);


struct sockaddr_un client_addr;
int client_fd;
socklen_t clen = sizeof(client_addr);
client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &clen);
```
### Lado cliente
Se conecta a un socket remoto que ya está en `listen()`.

```c
// cliente.c
int fd = socket(AF_UNIX, SOCK_STREAM, 0);

struct sockaddr_un addr;
memset(&addr, 0, sizeof(addr));
addr.sun_family = AF_UNIX;
strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

connect(fd, (struct sockaddr *)&addr, sizeof(addr));
```

### Envío y recepción (ambos lados)

```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```
### Atender a varios clientes a la vez

Con llamadas bloqueantes, un solo `accept()`/`recv()` te deja pegado a un cliente mientras otro espera. Tres soluciones, de peor a mejor:


```c
for (;;) {
    int client_fd = accept(listen_fd, ...);
    if (client_fd < 0) { if (errno == EINTR) continue; break; }
    if (fork() == 0) {
        close(listen_fd);           // el hijo no necesita el socket de escucha
        handle_client(client_fd);
        exit(EXIT_SUCCESS);
    }
    close(client_fd);               // el padre no necesita el socket del cliente
}
```
---
## 4. Señales: sincronización basada en señales
Estas funciones se usan siempre juntas para el mismo propósito: que un proceso se quede **dormido sin gastar CPU** hasta que ocurra algo (en vez de busy-waiting), y que ese "algo" sea comunicado por otro proceso con una señal. El combo típico es: instalar un handler con `signal()`, bloquear esa señal con `sigprocmask()`, y dormir con `pause()` o `sigsuspend()`.

### Handler y envío: `signal()` + `kill()`

```c
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
int kill(pid_t pid, int sig);
```

`signal()` instala qué función correr cuando llega `signum`. Dentro de un handler **sólo** se pueden usar funciones _async-signal-safe_ — nunca `printf()`, usar `write()` en su lugar. `kill()` (mal nombrado por historia) manda cualquier señal a cualquier PID, no sólo para matar.

```c
// relay_espejo.c
volatile int signal_received = 0;
void handler(int sig) { (void)sig; signal_received = 1; }   // sólo levanta un flag

signal(SIGUSR1, handler);
while (!signal_received)
    pause();                         // dormir hasta que llegue la señal
```

### Dormir hasta que pase algo: `pause()` vs `sigsuspend()`

```c
int pause(void);                      /* duerme hasta CUALQUIER señal */
int sigsuspend(const sigset_t *mask); /* reemplaza la máscara, duerme, y la restaura al volver */
```

`pause()` alcanza cuando no hay condición de carrera entre "consultar el flag" y "quedarme dormido". Si esa carrera existe (la señal puede llegar _entre_ el chequeo y el `pause()`), la solución correcta es bloquear la señal antes, y usar `sigsuspend()` para destaparla atómicamente sólo mientras se duerme.

### Máscaras de señales: `sigprocmask()` + `sigemptyset()`/`sigaddset()`

```c
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

Se arma el conjunto con `sigemptyset`+`sigaddset`, y `sigprocmask()` lo aplica: bloquear (`SIG_BLOCK`), desbloquear (`SIG_UNBLOCK`) o fijar (`SIG_SETMASK`). Es la forma de decirle al proceso "no me interrumpas con esta señal todavía" para armar una sección atómica alrededor de `pause`.

```c
// cadenamando.c — patrón completo: armar máscara, bloquear, y esperar
// con sigsuspend en un loop (evita perder señales entre el chequeo y el sleep)
sigset_t mascara_bloqueada, mascara_original;
sigemptyset(&mascara_bloqueada);
sigaddset(&mascara_bloqueada, SIGUSR1);
sigprocmask(SIG_BLOCK, &mascara_bloqueada, &mascara_original);

signal(SIGUSR1, manejador_alguien_paso);
signal(SIGCHLD, manejador_hijo_termino);

for (;;) {
    sigsuspend(&mascara_original);   // duerme con SIGUSR1 desbloqueada un instante
    if (señal == SIGUSR1) { /* ... */ }
    if (señal == SIGCHLD) { waitpid(...); }
    señal = 0;
}
```

> `sigaction()` es la versión moderna y más portable de `signal()` (permite especificar flags, máscara adicional durante el handler, etc.). Las guías de la materia aceptan `signal()`, que alcanza para estos ejercicios.

---
## 5. Sincronización:

### Semáforos: `wait()` / `signal()`

```c
sem(unsigned int value);   // crea un semáforo con valor inicial `value`
void wait();                 // si value <= 0, bloquea; si no, value-- (atómico)
void signal();                // value++, y despierta a alguno de los bloqueados

wait(s):    while (s <= 0) dormir();  s--;
signal(s):  s++;  if (alguien espera por s) despertar a alguno;
```

```c
mutex = sem(1)    // arranca "abierto": 1 solo puede pasar
// en cada proceso que toca la memoria compartida:
mutex.wait()
SECCIÓN_CRÍTICA()
mutex.signal()
```
Todos los procesos que compiten por el mismo recurso deben mirar el **mismo** semáforo (dos mutex distintos no sirven para protegerse entre sí).

**Signaling — "A pasa antes que B"**
```c
permiso = sem(0)            // arranca "cerrado": nadie puede pasar todavía
// Proceso A:                   // Proceso B:
A1()                          B1()
permiso.signal()              permiso.wait()
                               B2()          // B2 sólo corre después de A1
```

**Rendezvous / Barrera — "A1 antes que B2, y B1 antes que A2"**

```c
permisoA = sem(0); permisoB = sem(0)
// Proceso A:                   // Proceso B:
A1()                          B1()
permisoB.signal()             permisoA.signal()
permisoA.wait()                permisoB.wait()
A2()                          B2()
```

Ojo con el orden de `signal`/`wait` en cada lado: `signal()` antes de `wait()` en ambos evita el deadlock (si los dos hicieran `wait()` primero, ninguno soltaría el `signal()` que el otro necesita).

**Barrera con turnstile — "N procesos, nadie sigue hasta que lleguen todos"**
```c
barrera = sem(0); mutex = sem(1); counter = 0

ProcesoEstudiantes():
    implementarTp()
    mutex.wait()
    counter++
    if (counter == n): barrera.signal()   // el último abre la puerta
    mutex.signal()
    barrera.wait()
    barrera.signal()          // "reenvía" la apertura al siguiente que esperaba
    experimentar()
```

Variante con _múltiples signals_ (evita la cadena de reenvíos): el último en llegar hace `barrera.signal(n)` (n señales) en vez de que cada uno reenvíe la suya.
### Deadlock: qué mirar
- **Espera circular**: cada proceso espera un recurso que tiene el siguiente en una cadena cerrada.
- **No liberación**: un proceso retiene recursos mientras espera otros.
- **Starvation**: un proceso espera indefinidamente (por ejemplo, por mala suerte con las prioridades) — no es lo mismo que deadlock, pero es otro síntoma a buscar. Se mitiga con **aging** (subir gradualmente la prioridad de quien espera mucho).
- Hold and wait, procesos que ya tienen un recurso pueden solicitar otro
- exclusion mutua, un recurso no puede estar asignado a mas de un proceso.
---
## 6. Scheduling (planificación de CPU)
- **Ráfagas (bursts) de CPU e I/O**: la ejecución de un proceso alterna ráfaga de CPU → ráfaga de I/O → CPU → I/O... Un proceso _intensivo en I/O_ tiene muchas ráfagas de CPU cortas; uno _intensivo en CPU_ tiene pocas ráfagas largas.
- **Nonpreemptive**: sin desalojo; **Preemptive**: con desalojo 

### Métricas (fórmulas exactas)

| Métrica           | Fórmula                                                        | Objetivo    |
| ----------------- | -------------------------------------------------------------- | ----------- |
| Uso de CPU        | —                                                              | Maximizar ↑ |
| Throughput        | procesos terminados / unidad de tiempo                         | Maximizar ↑ |
| **Turnaround**    | `instante en que termina − instante en que llegó`              | Minimizar ↓ |
| **Waiting time**  | `turnaround − ráfaga de CPU` (suma de los períodos en _ready_) | Minimizar ↓ |
| **Response time** | `instante en que corre por primera vez − llegada`              | Minimizar ↓ |

TAT (Fin - Llegada) | WT (TAT-RAFAGA)

**FCFS (First-Come, First-Served)** Se da la CPU al primero que la pide, en orden de llegada. Nonpreemptive. Problema: **convoy effect** .Malo para sistemas interactivos.

**Round-Robin (RR)** Cola _ready_ en orden de llegada. Si no termina su ráfaga en ese quantum, se lo desaloja y va al final de la cola.
- Es la política preferida para sistemas **interactivos** (mejor response time), a costa de más cambios de contexto.

**Shortest-Job-First (SJF)** Se elige, entre los que ya llegaron, al de ráfaga de CPU más corta. Nonpreemptive.

**Shortest-Remaining-Time-First (SRTF)** Versión _preemptive_ de SJF.

**Prioridades** Cada proceso tiene una prioridad; se ejecuta el de mayor prioridad en _ready_ (empates → FCFS). Puede ser preemptive o no. Riesgo: **starvation** de los procesos de baja prioridad si llegan constantemente procesos de alta prioridad → se mitiga con **aging**. 

**Multilevel Queue** Colas separadas por prioridad, cada una con prioridad absoluta sobre las de menor prioridad. La prioridad (y por lo tanto la cola) de cada proceso es **estática**: nace en una cola y no cambia. Dentro de cada cola suele usarse RR. Útil para separar tipos de carga (real-time > interactivos > batch), pero puede sufrir starvation de las colas bajas si la cola alta nunca se vacía.

**Multilevel Feedback Queue (MLFQ)** Igual que Multilevel Queue, pero los procesos **pueden cambiar de cola** dinámicamente (típicamente: a mayor uso de CPU, más baja la prioridad). Puede implementar aging para evitar starvation. Termina favoreciendo a los procesos intensivos en I/O e interactivos (ráfagas de CPU más cortas).

**Earliest-Deadline-First (EDF)** — scheduling de tiempo real Prioridad dinámica según el _deadline_: cuanto más cercano, más prioridad (preemptive: llega un proceso, se compara su deadline con el que está corriendo). En teoría es óptimo (si la instancia es factible, todos cumplen su deadline); en la práctica un deadline vencido no se puede recuperar — los sistemas _hard real-time_ corren un test de planificabilidad antes de aceptar una tarea nueva.
### Tiempo real: soft vs. hard
- **Soft real-time**: no garantiza _cuándo_ corre un proceso RT, sólo que corre con más prioridad que los no críticos.
- **Hard real-time**: una tarea crítica debe ejecutar dentro de su _deadline_; ejecutarla tarde equivale a no haberla ejecutado.
