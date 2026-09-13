Ejercicio 8

Considerar cada uno de los siguientes enunciados. Para cada caso, escribir el código que permita la ejecución de los procesos según la forma de sincronización planteada utilizando semáforos (no olvidar los valores iniciales). Se debe argumentar porqué cada solución evita la inanición:

Se tienen tres procesos (A, B y C). Se desea que el orden en que se ejecutan sea el orden alfabético, es decir que las secuencias normales deben ser: ABC, ABC, ABC, ...

Idem anterior, pero se desea que la secuencia normal sea: BBCA, BBCA, BBCA, ...

Se tienen un productor (A) y dos consumidores (B y C) que actúan no determinísticamente. La información provista por el productor debe ser retirada siempre 2 veces, es decir que las secuencias normales son: ABB, ABC, ACB o ACC. Nota: ¡Ojo con la exclusión mutua!  

Se tienen un productor (A) y dos consumidores (B y C). Cuando C retira la información, la retira dos veces. Los receptores actúan en forma alternada. Secuencia normal: ABB, AC, ABB, AC, ABB, AC...

ABC
```c
/// Shared
S=[semA(1),semB(0),semC(0)]

/// Funcion // i parametro, A=0,B=1,C=2
while(1){
    S[i].wait();
    //proc
    S[(i+1) % 3].signal();
}   
```

BBCA
```c
/// Shared
// Inicialización: Arranca B con luz verde
semA = 0;
semB = 1; 
semC = 0;

// --- Proceso A ---
while(1) {
    semA.wait();
    // Ejecuta A
    semB.signal(); // Le pasa la posta a B
}

// --- Proceso B ---
int cuenta = 0; // Variable local de B
while(1) {
    semB.wait();
    // Ejecuta B
    
    cuenta++;
    if (cuenta == 2) {
        cuenta = 0; // Reseteo para la próxima vuelta
        semC.signal(); // Ya corrí dos veces, le paso la posta a C
    } else {
        semB.signal(); // Corrí una sola vez, me auto-habilito para correr de nuevo
    }
}

// --- Proceso C ---
while(1) {
    semC.wait();
    // Ejecuta C
    semA.signal(); // Le pasa la posta a A
}
```


ABB, ABC, ACB o ACC
```c
sem_productor(1);
producto_ready(0);
mutex_consumir(1);
count =0;

// --- Productor ---
while(1)
    sem_productor.wait();
    //producir
    producto_ready.signal();
    producto_ready.signal();


// --- Consumidores ---
while(1):
    producto_ready.wait();
    mutex_consumur.wait();
    count++
    // Consumir
    if (count==2)
        count=0;
        sem_productor.signal();
    mutex_consumur.signal();
```    


ABB, AC, ABB, AC, ABB, AC...
```c
sem_productor(1);
consumidorB(0);
consumidorC(0);
turn='b';
// --- Productor ---
while(1)
    sem_productor.wait();
//producir
    if(turn=='b')
        consumidorB.signal();
    else:
        consumidorC.signal();

// --- Consumidor B ---
countb = 0;
while(1):
    consumidorB.wait();
    countb++;
    // Consumir
    if (countb==2)
        countb = 0;
        turn='c'
        sem_productor.signal();
    else:
        consumidorB.signal();

// --- Consumidor C ---
while(1):
    consumidorC.wait();
    // Consumir
    turn ='b'
    sem_productor.signal();
```