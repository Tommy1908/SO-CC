5.  Se tienen n procesos: P_1, P_2, ... , P_n que ejecutan el siguiente código.
    Se espera que todos los procesos terminen de ejecutar la función preparado() antes de que alguno de ellos llame a la función critica().
    ¿Por qué la siguiente solución permite inanición?
    Modificar el código para arreglarlo.

```
preparado()

mutex.wait()
count = count + 1
mutex.signal()

if (count == n)
    barrera.signal()

barrera.wait()

critica()
```

Supongamos que entran y todos hacen count + 1
El proceso 1 tiene el 1, el 2 el 2, y asi, porque asumo que arranca en 0
Ninguno hasta el proceso n, entra al if.
Esto hace que todos menos el ultimo esten bloqueados en la barrera (arranca en 0).
El ultimo proceso libera 1 "cupo" y pasa alguno de los n, luego n-1 procesos se "starvean".

Bastaria para arreglarlo

```
preparado()

mutex.wait()
count = count + 1

if (count == n)
    for(int i = 0; i < n; i++){
        barrera.signal()
    }
mutex.signal()


barrera.wait()

critica()
```

Liberamos n cupos, ademas, dentro del mutex, para que si en algun caso intermedio de la suma, no liberemos n cupos mas de 1 vez.
