/*
 * Ejercicio 2: One ring to rule them all (template).
 *
 * Se corre con: make run-anillo N=<n> S=<s> C=<c> P=<p>
 *
 *   n : cantidad de procesos del anillo (n >= 3)
 *   s : cuál de los hijos es el distinguido, el que arranca (1 <= s <= n)
 *   c : valor inicial del mensaje
 *   p : cota; el anillo para cuando el distinguido recibe un valor >= p (p > c)
 *
 * El parseo y la validación de los parámetros ya están resueltos: lo que
 * falta es armar el anillo de pipes, crear los hijos y programar la
 * ronda.
 */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

static void usage(const char *prog)
{
	fprintf(stderr, "uso: %s <n> <s> <c> <p>\n", prog);
	fprintf(stderr, "  n : cantidad de procesos del anillo (n >= 3)\n");
	fprintf(stderr, "  s : hijo distinguido, el que arranca (1 <= s <= n)\n");
	fprintf(stderr, "  c : valor inicial del mensaje\n");
	fprintf(stderr, "  p : cota, tiene que ser mayor que c\n");
}

/* strtol con todos los chequeos puestos: que haya algún dígito, que no
 * sobre basura al final y que entre en un int. */
static int parse_int(const char *text, int *out)
{
	char *end;
	long value;

	errno = 0;
	value = strtol(text, &end, 10);
	if (end == text || *end != '\0')
		return -1;
	if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
		return -1;

	*out = (int)value;
	return 0;
}

int main(int argc, char **argv)
{
	int n, s, c, p, leader;

	if (argc != 5) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	if (parse_int(argv[1], &n) < 0 || parse_int(argv[2], &s) < 0 ||
	    parse_int(argv[3], &c) < 0 || parse_int(argv[4], &p) < 0) {
		fprintf(stderr, "error: los cuatro parámetros tienen que ser números enteros\n");
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	if (n < 3) {
		fprintf(stderr, "error: el anillo necesita al menos 3 procesos (n = %d)\n", n);
		return EXIT_FAILURE;
	}
	if (s < 1 || s > n) {
		fprintf(stderr, "error: s tiene que estar entre 1 y %d (s = %d)\n", n, s);
		return EXIT_FAILURE;
	}
	if (p <= c) {
		fprintf(stderr, "error: p tiene que ser mayor que c (c = %d, p = %d)\n", c, p);
		return EXIT_FAILURE;
	}

	/* Adentro trabajamos con índices desde 0; el enunciado numera los
	 * hijos desde 1, y así se imprimen. */
	leader = s - 1;

	/* TODO: crear los pipes del anillo. */

	/* TODO: crear los n hijos. */

	/* TODO: programar el trabajo de cada hijo (te va a quedar más prolijo
	 *       en una función aparte).
	 */

	/* TODO: inyectar el valor inicial. */

	/* TODO: Recibir el valor final del distinguido, esperar a que terminen
	 *       todos los hijos e imprimir el resultado final.
	 */

	return EXIT_SUCCESS;
}