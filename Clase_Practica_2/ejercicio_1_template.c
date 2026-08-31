#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() write() read()
#include "dado.h"    // tirar_dado()

// Constantes 0 / 1 para READ / WRITE
enum { READ, WRITE };
// Constantes 0 / 1 para LESTER / ELIZA
enum { LESTER, ELIZA };

// Variables globales
int pipesEliza[2];
int pipesLester[2];

void lester() {
  
  // Tiro el dado
  int dado_lester = tirar_dado();
  printf("Soy Lester (%d) y tire %d\n", getpid(), dado_lester);
  // Le informo el resultado a Humberto
  write(pipesLester[WRITE], &dado_lester, sizeof(dado_lester));
  exit(0);
}

void eliza() {
  // Tiro el dado
  int dado_eliza = tirar_dado();
  printf("Soy Eliza (%d) y tire %d\n", getpid(), dado_eliza);

  // Le informo el resultado a Humberto
  write(pipesEliza[WRITE], &dado_eliza, sizeof(dado_eliza));
  exit(0);
}

int main(int argc, char const* argv[]) {
  // Creo los pipes
  pipe(pipesEliza);
  pipe(pipesLester);

  printf("Soy Humberto (%d)\n", getpid());  
  // Creo a Lester
  int pid_lester = fork();
  if(pid_lester==0) lester();
  // Creo a Eliza
  int pid_eliza = fork();
  if(pid_eliza==0) eliza();


  // Recibo el dado de Lester
  int resultado_lester;
  read(pipesLester[READ], &resultado_lester, sizeof(resultado_lester));

  // Recibo el dado de Eliza
  int resultado_eliza;
  read(pipesEliza[READ], &resultado_eliza, sizeof(resultado_eliza));

  // Anuncio al ganador
  if (resultado_eliza > resultado_lester)      printf("Gano Eliza\n");
  else if (resultado_eliza < resultado_lester) printf("Gano Lester\n");
  else                                         printf("Empate\n");

  return 0;
}
