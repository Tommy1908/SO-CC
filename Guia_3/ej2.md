Un sistema con 4 procesos accediendo a una variable compartida x y un mutex.
Los 4 procesos ejecutan el siguiente código.
Se debe asegurar que cada vez que un proceso lee la variable compartida,
previamente solicita el mutex y luego lo libera

```
x = 0; // Variable compartida
mutex(1); // Mutex compartido

while (1) {
    mutex.wait();
    y = x; // Lectura de x
    mutex.signal();

    if (y <= 5) {
        x++;
    } else {
        x--;
    }
}

```

Supongamos que el proceso 1 empieza, entra al mutex, lee x, sale, ejecuta y suma 1 a x.
Continua la ejecucion de a, y vuelve a sumar 1 a x. Una vez mas continua y lee x, lo guarda en y, pero al salir del mutex, le toca al proceso 2. Lee x, sale. le toca al proceso 3, sale, y el proceso 4 lee x y sale.
El proceso 1,2,3,4 tiene y = 2, cada uno le toca su tuirno y suma 1. ahora x = 6.
Si Solo hubiera ejecutado a, valdria 4.
Evidentemente hay una race condition
