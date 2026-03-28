# include <unistd.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
//gcc ej5.c -o ej5

void wait_for_child(pid_t child) {
    waitpid(child, NULL, 0); 
}

int a(){
    //Ej A
		/*
    Abraham es padre de Homero, Homero es padre de Bart, Homero es padre de Lisa, Homero es padre de Maggie. 
    Cada proceso debe imprimir por pantalla el nombre de la persona que representa.
		*/
	printf("Abraham (PID: %d, parent:%d) \n", getpid(), getppid());
	//Abraham crea homero
  pid_t fork_pid_homero = fork();
  if(fork_pid_homero == 0){
		printf("Homero (PID: %d, parent:%d) \n", getpid(), getppid());
		//Homero crea Bart
		pid_t fork_pid = fork();
  	if(fork_pid == 0){
			printf("Bart (PID: %d, parent:%d) \n", getpid(), getppid());
			exit(0);
		}
		else{
			//Homero crea Lisa
			pid_t fork_pid = fork();
  		if(fork_pid == 0){
				printf("Lisa (PID: %d, parent:%d) \n", getpid(), getppid());
				exit(0);
			}
			else{
				//Homero crea Maggie
				pid_t fork_pid = fork();
  			if(fork_pid == 0){
					printf("Maggie (PID: %d, parent:%d) \n", getpid(), getppid());
					exit(0);
				}
			}
		}
		printf("Este es el print final de Homero\n");
		exit(0);
	}
	printf("Ver la terminal es normal si no pongo el wait en main, Abraham termina mientras homero aun esta haciendo cosas\n");
	printf("Este es el print final de A\n");
	return fork_pid_homero;
}

int b(){
    //Ej b
		/*
    Abraham es padre de Homero, Homero es padre de Bart, Homero es padre de Lisa, Homero es padre de Maggie. 
    Cada proceso debe imprimir por pantalla el nombre de la persona que representa.
		*/
	printf("Abraham (PID: %d, parent:%d) \n", getpid(), getppid());
	//Abraham crea homero
  pid_t fork_pid_homero = fork();
  if(fork_pid_homero == 0){
		printf("Homero (PID: %d, parent:%d) \n", getpid(), getppid());
		//Homero crea Bart
		pid_t fork_pid_bart = fork();
  	if(fork_pid_bart == 0){
			printf("Bart (PID: %d, parent:%d) \n", getpid(), getppid());
			exit(0);
		}

		//Homero crea Lisa
		pid_t fork_pid_lisa = fork();
  	if(fork_pid_lisa == 0){
			printf("Lisa (PID: %d, parent:%d) \n", getpid(), getppid());
			exit(0);
		}
		//Homero crea Maggie
		pid_t fork_pid_maggie = fork();
  	if(fork_pid_maggie == 0){
			printf("Maggie (PID: %d, parent:%d) \n", getpid(), getppid());
			exit(0);
		}
		wait_for_child(fork_pid_bart);
		wait_for_child(fork_pid_lisa);
		wait_for_child(fork_pid_maggie);
		printf("Este es el print final de Homero\n");
		exit(0);
	}
	wait_for_child(fork_pid_homero);
	printf("Este es el print final de B\n");
	return fork_pid_homero;
}

int main(){

		printf("Ejercicio A\n");
		pid_t ult_proceso_a = a();
		wait_for_child(ult_proceso_a); //esto para ordenar los prints
		printf("\n");
		printf("Ejercicio B\n");
		b();

	return 0;
}
