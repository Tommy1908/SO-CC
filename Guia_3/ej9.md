Ejercicio 9  

Suponer que se tienen N procesos Pi, cada uno de los cuales ejecuta un conjunto de sentencias ai y bi. ¿Cómo se pueden sincronizar estos procesos de manera tal que los bi se ejecuten después de que se hayan ejecutado todos los ai?

```c
// Memoria compartida
int <atomic> ready = 0;
int n; // Procesos
sem barrera(0);

// --- Pi ---
ai()

ready.inc()

if(ready==n)
    barrera.signal()

barrera.wait()
barrera.signal()

bi()
```

Sin atomic

```c
// Memoria compartida
int n; // Procesos
int ready = 0;
sem barrera(0);
sem mutex(1);

// --- Pi ---
ai()

mutex.wait();
ready++;
if (ready == n)
    barrera.signal();
mutex.signal();

barrera.wait()
barrera.signal()

bi()

```

Si hubiera un while inifinito de ai, bi, ai, bi, el molinete rompe, so...

```c
// Memoria compartida
int n; // Procesos
int ready = 0;
sem barrera_in(0);
sem barrera_out(1);
sem mutex(1);

// --- Pi ---
while(1)
    ai()
    
    mutex.wait();
    ready++;
    if (ready == n)
        barrera_out.wait(); // Come la señal suelta
        barrera_in.signal();
    
    mutex.signal();
    
    //Entrada   
    barrera_in.wait();
    barrera_in.signal();
    
    bi()

    //Salida
    mutex.wait();
    ready--;
    if (ready == 0)
        barrera_in.wait(); // Come la señal suelta
        barrera_out.signal(); 
    mutex.signal();
    
    barrera.out.wait();
    barrera_out.signal();
```

Ya que estamos, uno sin molinete...

```c
// Memoria compartida
int n; // Procesos
int ready = 0;
sem barrera_in(0);
sem barrera_out(0);
sem mutex(1);

// --- Pi ---
while(1)
    ai()
    
    mutex.wait();
    ready++;
    if (ready == n)
        for(int i = 0; i < n; i++)
            barrera_in.signal();
    
    mutex.signal();
    
    //Entrada   
    barrera_in.wait();
    
    bi()

    //Salida
    mutex.wait();
    ready--;
    if (ready == 0)
        for(int i = 0; i < n; i++)
            barrera_out.signal(); 
    mutex.signal();
    
    barrera_out.wait();
```