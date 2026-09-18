# Taller de syscalls y señales

> El taller se compila y se ejecuta adentro de una VM Linux que levanta el
> propio `make`. Si todavía no la preparaste, arrancá por **[VM.md](https://github.com/SSOO-Exactas-2026-2C/taller-tools/blob/main/VM.md)**:
> ahí está el setup (`make install`, `make start-vm`, `make shell`) y los
> comandos generales del entorno.

## Ejercicio 1: Análisis de sincronización básica con señales

### Enunciado

Se proporciona un programa binario llamado `relay` cuyo uso es:

```bash
make run-relay N=3
```

donde `N` es un número entero entre 1 y 5.

### Parte A: Análisis con `strace`

Analizá el comportamiento del binario `relay` con `strace`:

```bash
make strace-relay N=3
make strace-relay N=5
```

El comando muestra las syscalls relacionadas con señales y procesos, y deja la
traza completa en `trace.txt`. Si querés ver todo por pantalla, pasale otro
filtro: `make strace-relay N=3 FILTRO=.`

Luego, respondé las siguientes preguntas:

**a)** ¿Cuántos procesos se lanzan en total? Identificá la relación padre-hijo a partir de los `clone`/`fork` observados en la salida de `strace`.

**b)** ¿Qué señales se intercambian entre los procesos? ¿Cuáles son los PIDs emisor y receptor de cada `kill`?

**c)** ¿Cuál es el orden en que se ejecutan y terminan los procesos hijos? ¿Observás sincronización explícita entre ellos?

**d)** ¿Qué syscalls relacionadas con manejo de señales se invocan (por ejemplo: `signal`, `pause`, `kill`, `wait`)? ¿En qué orden?

**e)** Analizá la salida textual del programa. ¿Qué mensaje imprime cada proceso? ¿En qué orden aparecen los mensajes?

### Parte B: Replicación del programa

Escribí en `src/ej1/relay_espejo.c` un programa en C que replique **exactamente** el comportamiento del binario `relay`. Tu programa debe:

- Aceptar el mismo formato de parámetros: `./bin/relay_espejo <N>`.
- Producir la misma salida textual en el mismo orden.
- Utilizar las mismas syscalls de sincronización observadas en `strace`.
- Hacer uso de `fork()`, `signal()`, `kill()`, `pause()`, y `wait()`.

#### Validación de tu solución

```bash
make test-ej1
```

Los tests comparan tu salida contra la de `relay` para N=1..5 (normalizando los
PIDs, que obviamente cambian), chequea que rechaces los parámetros inválidos
igual que el original, verifica en la traza de `strace` que uses las syscalls
pedidas, y controla que no queden procesos huérfanos.

Para comparar a mano:

```bash
make strace-relay N=3     # traza del original
make strace-ej1 N=3       # traza de la tuya
```

---

## Ejercicio 2: La cadena de mando

### Enunciado

Escribir en `src/ej2/cadenamando.c` un programa en C que reciba 3 parámetros de entrada:

- **N**: cantidad de procesos a crear (menor a 10)
- **K**: cantidad de rondas
- **J**: el identificador del "líder maldito" ($0 \le J < N$)

El programa debe crear N procesos hijo, cada uno identificado con un número de 0 a N-1, organizados en una cadena de sucesión (el hijo $i$ conoce quién es su sucesor $i+1$). El juego se repite durante K rondas, y en cada ronda ocurre lo siguiente:

#### Dinámica de cada ronda

**a)** El padre le da inicio a la ronda pasándole "el mando" al hijo 0.

**b)** El hijo que tiene el mando genera al azar un número entre 0 y N-1. Si el número coincide con **J**, ese hijo expresa sus últimas palabras por salida estándar y termina su ejecución. En caso contrario, el hijo le pasa el mando a su sucesor vivo dentro de la cadena, y queda a la espera.

**c)** El padre debe enterarse, en cuanto ocurra, de cada hijo que termina su ejecución, para poder actualizar la cadena de sucesión y descartarlo de las rondas siguientes.

#### Requerimientos adicionales

A lo largo de todo el juego, el programa debe llevar la cuenta de **cuántas veces fue pasado el mando en total** (contando todas las rondas). Cada proceso involucrado (el padre y cada uno de los hijos) debe, al momento de expresar sus últimas palabras o al finalizar, reportar por salida estándar cuál es el valor de esa cuenta **tal como él la conoce**.

Al finalizar las K rondas (o al quedar un solo sobreviviente, lo que ocurra primero), el padre debe:

1. Notificar qué hijos sobrevivieron (identificador y PID)
2. Terminar la ejecución de los hijos restantes
3. Informar el valor final de su propia cuenta de pases de mando

Queda a criterio de cada grupo decidir con qué mecanismos concretos se implementa cada una de estas partes, respetando las restricciones de la siguiente sección.

### Restricciones

- **Toda la comunicación y coordinación entre procesos debe resolverse exclusivamente mediante señales** y las llamadas de gestión de procesos correspondientes (`fork`, la familia de `wait`, etc.).
- **No se permiten pipes, memoria compartida, ni sockets**, ni ningún otro mecanismo de comunicación entre procesos por fuera de señales.

### Formato de salida

Las pruebas verifican estas tres líneas, así que respetá el formato al pie de la letra:

```
HIJO <id> PID <pid> ULTIMAS_PALABRAS mando_total=<valor>
SOBREVIVIENTE <id> PID <pid>
PADRE mando_total=<valor>
```

Para validar:

```bash
make test-ej2
```

---

## Comandos del taller

| Comando | Qué hace |
|---|---|
| `make run-relay N=3` | Corre el binario `relay` de la cátedra |
| `make strace-relay N=3` | Traza las syscalls de `relay` (**Parte A del ejercicio 1**) |
| `make build-ej1` | Compila tu `src/ej1/relay_espejo.c` |
| `make run-ej1 N=3` | Corre tu implementación del ejercicio 1 |
| `make strace-ej1 N=3` | Traza **tu** implementación, para comparar con la de `relay` |
| `make test-ej1` | Tests del ejercicio 1 |
| `make build-ej2` | Compila tu `src/ej2/cadenamando.c` |
| `make run-ej2 N=4 K=5 J=1` | Corre tu implementación del ejercicio 2 |
| `make strace-ej2 N=4 K=5 J=1` | Traza tu implementación del ejercicio 2 |
| `make test-ej2` | Tests del ejercicio 2 |

---

## Entrega

### Ejercicio 1

- `src/ej1/relay_espejo.c` — Código que replica el comportamiento de `relay`.

### Ejercicio 2

- `src/ej2/cadenamando.c` — Implementación de la cadena de mando.

### Ejecución esperada

```bash
make run-ej1 N=3
make run-ej2 N=4 K=5 J=1   # N=4 hijos, K=5 rondas, J=1 (maldito)
```

---

## Materiales provistos

- **`bin/relay`** — Binario compilado del programa misterio (ejercicio 1).
- **`src/ej1/relay_espejo.c`** y **`src/ej2/cadenamando.c`** — Templates con TODOs.

---

## Recomendaciones

### Para ejercicio 1

- Usá `strace` extensamente. Los syscalls son tu guía.
- Testeá con diferentes valores de N (1, 3, 5).
- No te conformes con "parece funcionar"; usá `make test-ej1`.

### Para ejercicio 2

- Diseñá la arquitectura en papel antes de escribir código.
  - ¿Cómo se notifica al padre de una muerte? (SIGCHLD)
  - ¿Cómo conocen los hijos quién es su sucesor?
  - ¿Cómo se actualiza la cadena dinámicamente?

- **La primer ronda es la más importante:** asegurate de que funciona perfectamente antes de iterar K veces.

- Testeá casos borde:
  - ¿Qué pasa si N=1? (Solo el padre y un hijo)
  - ¿Qué pasa si J=0? (El primer hijo es maldito)
  - ¿Qué pasa si K es muy grande?

- Usá `make strace-ej2` para debuggear la sincronización.

- **No hagas printf() dentro de signal handlers.** Usá `write()` siempre.

---

## Debugging y herramientas útiles

Con `make shell` (ver [VM.md](https://github.com/SSOO-Exactas-2026-2C/taller-tools/blob/main/VM.md)) entrás a una terminal adentro de la VM,
parada en la carpeta del taller. Desde ahí tenés todo Linux disponible:

```bash
# Ver procesos vivos
ps -ef | grep cadenamando

# Ver procesos zombies
ps -ef | grep defunct

# Matar procesos colgados
pkill -f cadenamando

# Trazas a medida
strace -f -e trace=signal ./bin/relay 3
strace -f -e trace=%signal,%process ./bin/cadenamando 4 5 1

# Ejecutar múltiples veces y comparar salidas
for i in $(seq 1 5); do ./bin/cadenamando 3 2 0 > out_$i.txt; done
diff -u out_1.txt out_2.txt
```

---

## Preguntas frecuentes

**P: ¿Puedo usar `sigaction()` en lugar de `signal()`?**
R: Sí, es incluso mejor. `sigaction()` es más portable y seguro. Pero `signal()` alcanza para estos ejercicios.

**P: ¿Qué pasa si un proceso muere sin imprimir su último mensaje?**
R: Es un problema de carrera. Asegurate de que cada proceso imprime ANTES de salir. Usá `fflush()` después de cada `printf()`.

**P: ¿Cómo sé si mis contadores están correctos en La Cadena?**
R: Ejecutá varias veces la misma entrada. Si los contadores del padre cambian entre ejecuciones, probablemente haya un bug de sincronización.

**P: ¿Puedo usar `sleep()` para hacer esperar a los procesos?**  
R: Para debugging, sí. Pero recordá que `sleep()` se interrumpe si el proceso recibe una señal (no cualquiera, hay ciertas condiciones) y retorna el tiempo restante. Para sincronización real, usá `pause()`.

**P: Se me cuelga el programa, ¿cómo lo mató?**
R: `Ctrl+C` en la terminal. Si quedaron procesos dando vueltas adentro de la VM: `make shell` y después `pkill -f cadenamando`. En el peor caso, `make stop-vm` se lleva todo puesto.

**P: ¿Los PIDs deben coincidir exactamente entre ejecuciones?**
R: No. Los PIDs son arbitrarios. Lo importante es la estructura (padre→hijo) y la salida textual.

Los problemas del entorno (la VM no arranca, falta qemu, etc.) están en
[VM.md](https://github.com/SSOO-Exactas-2026-2C/taller-tools/blob/main/VM.md).

---

## Referencias útiles

- **man 7 signal** — Tabla de señales y propiedades (signal-safety list)
- **man 7 signal-safety** — Funciones que pueden llamarse desde un handler
- **man 2 fork**, **man 2 kill**, **man 2 wait**, **man 2 pause**, **man 2 signal**
