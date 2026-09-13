Se tienen N procesos, P0, P1, ..., PN-1 (donde N es un parámetro). Se requiere sincronizarlos de manera que la secuencia de ejecución sea Pi, Pi+1, ..., PN-1, P0, ..., Pi-1 (donde i es otro parámetro). Escribir el código que deben ejecutar cada uno de los procesos para cumplir con la sincronización requerida utilizando semáforos (no olvidar los valores iniciales).

Asumo que cada programa conoce su i(?) sino no se. 
j va ser su numero
```c
semaforos[n]; //Un arreglo de I semaforos inicializados en 0. Exepto el Pi, que empieza en 1
// [sem(0),sem(0),sem(0),..(i)sem(1), sem(0),sem(0),sem(0)]
//Shared

//Programa
semaforos[j].wait();
//Ejecuccion
semaforos[(j+1)%n].signal();
```