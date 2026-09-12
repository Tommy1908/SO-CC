```
wait(s):
    while (s<=0) dormir();
    s--;

signal(s):
    s++;
    if(alguien espera por s) despertar a alguno;
```
`
Bueno si alguno no fuera atomico, podria pasar que si fuera un mutex (de 1)
y 2 procesos, el primero llame wait, s = 1, por lo tanto no duerme y se queda justo saliendo del while.
La comparacion es load, cmp y para el s--, es load, dec, store. supongamos que quedo antes del store.
Viene el procesos 2, entra, el s sigue valiendo 1 aun, hace todo, incluido s--, entra a la seccion critica, y luego el 1 continua, setea en 0 el s, (que ya estaba en 0 por el 2), y entra a la seccion critica, mientras el 2 tambien lo esta.

Tambien podria quedar que el 1 se corta antes de ellegar al s--, y venga el segundo, eso dejaria el s en -1 al final..y evitamos lo de las intrucciones assembler