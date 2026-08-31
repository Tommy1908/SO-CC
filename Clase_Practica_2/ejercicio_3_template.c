#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define RANGO_MIN 2L
#define RANGO_MAX 1000000001L

// Constantes 0 / 1 para READ / WRITE
enum { READ, WRITE };

int esPar(long numero) {
  return (numero & 1) == 0;
}

long contarPares(long desde, long hasta) {
  long cantidad = 0;
  for (long i = desde; i < hasta; ++i) {
    if (esPar(i)) {
      cantidad++;
    }
  }
  return cantidad;
}

void ejecutarHijo(int i /* file descriptors a los pipes necesarios */) {
  // Leer del i-ésimo pipe el rango [desde, hasta) para realizar el cómputo
  // Contar pares y escribir el resultado
}

int main(int argc, char const* argv[]) {
  // Parsear la cantidad de procesos
  if (argc != 2) {
    printf("Debe ejecutar con la cantidad de procesos N como parámetro.\n");
    printf("Ejemplo: %s N\n", argv[0]);
    return 1;
  }
  int N = atoi(argv[1]);

  // Crear pipes

  // Crear hijos

  // Calcular rangos para cada hijo
  // El intervalo es: [RANGO_MIN, RANGO_MAX) (es decir, cerrado-abierto)
  long cantidad = ((RANGO_MAX - RANGO_MIN) + (N - 1)) / N;
  long desde = RANGO_MIN;
  for (int i = 0; i < N; i++) {
    long hasta = desde + cantidad;
    if (hasta > RANGO_MAX) hasta = RANGO_MAX;

    // Enviar rango a cada hijo

    desde = hasta;
  }

  // Cerrar pipes inteligentemente

  // Leer los resultados de cada hijo
  long resultado = 0;
  printf("Resultado total: %ld\n", resultado);

  return 0;
}
