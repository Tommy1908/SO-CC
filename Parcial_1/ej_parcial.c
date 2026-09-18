#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Con n numero de hijo
void proceso_hijo(int n, int pipes[][2], int total_hijos){
    // Cerrar todos menos mi extremo de escritura y de lectura
    close(pipes[n*2][1]); // Cierro mi canal de escritura donde voy a leer del padre
    close(pipes[n*2+1][0]); // Cierro mi canal de lectura en mi canal donde voy a escribir
    for(int i=0; i<total_hijos; i++){
        if(i==n){
            continue;
        }
        close(pipes[i*2][0]);
        close(pipes[i*2][1]);
        close(pipes[i*2+1][0]);
        close(pipes[i*2+1][1]);
    }

    while(1){
        int indice;
        int bytes_read = read(pipes[n*2][0], &indice, sizeof(indice));
        if(bytes_read ==0){ // Llego un EOF (nadie mas escribe)
            break;
        }
        int valor = indice;
        //int valor = generarValor(indice);

        write(pipes[n*2+1][1], &valor, sizeof(valor));
    }
    //printf("%d, termine\n",getpid());
    exit(EXIT_SUCCESS);
}

// Los n son de ida del padre (escritura)
// Los n+1 son de ida del hijo (escritura)

int main(){
    int n=9; 
    int m=70;
    int arreglo[m];
    int hijos [n];
    int pipes[n*2][2];
	//int (*pipes)[2] = calloc(num_pipes, sizeof(int[2]));
    for(int i=0; i<n*2; i++){
        pipe(pipes[i]);
    }

    for(int i=0; i<n; i++){
        int pid = fork();
        if(pid == 0){
            proceso_hijo(i, pipes, n);
        }
        hijos[i]=pid; 
        close(pipes[i*2][0]); // Lectura del padre en su canal de escritura
        close(pipes[i*2+1][1]); // Escritura en el canal del hijo
    }

    //Opcion valida pero mas lenta aun, si el buffer fuera de solo 1 elemento
    //int val;
    //for(int i = 0; i < m; i++){
    //    write(pipes[((i%n)*2)][1], &i, sizeof(i)); // Le pido que valor quiero que calcule
    //    read(pipes[(i%n)*2+1][0], &val, sizeof(val)); // Leo su resultado, noto que si uno tardara mas, retrasaria al resto. Ej hijo 3 tarda 50 milisegundos, y el resto 1, el 4, 5... van a estar retrasados, pero esta es la solucion "equitativa" (nose si esta bien igual hacer esto)
    //    arreglo[i] = val;
    //}

    // asignar trabajo
    for(int i = 0; i < m; i++){
        write(pipes[((i%n)*2)][1], &i, sizeof(i)); 
    }

    //Vas leyendo, podrias trabarte aca y retrasar a todos, pero es equitativo
    int val;
    for(int i = 0; i < m; i++){
        read(pipes[(i%n)*2+1][0], &val, sizeof(val)); 
        arreglo[i] = val;
    }
    

    for(int i = 0; i < m; i++){
        printf("%d,",arreglo[i]); // Imprimimos el valor final
    }
    
    //printArray(arreglo); // Imprimimos el valor final



    // Cerrar los pipes (hace falta? estoy por hacer exit..pero bueno seamos prolijos)
    for(int i=0; i<n; i++){
        close(pipes[i*2][1]); // Escritura del padre en su canal de escritura
        close(pipes[i*2+1][0]); // Escritura en el canal del hijo
    }

    for(int i=0; i<n; i++){
        wait(NULL); // Limpiamos para que no haya zombies
    }
    exit(EXIT_SUCCESS);
}