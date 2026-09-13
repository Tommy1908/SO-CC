6.

Cambiar la solución del ejercicio anterior por una solución basada solamente en las herramientas atómicas vistas en las clases, que se implementan a nivel de hardware, y responder las siguientes preguntas:

a) ¿Cuál de las dos soluciones genera un código más legible?
b) ¿Cuál de ellas es más eficiente? ¿Por qué?
c) ¿Qué soporte requiere cada una de ellas del SO y del HW?

```
atomic<int> preparados;
preparados.set(0); // cantidad de procesos listos

atomic<int> barrera;
barrera.set(1); // 1 = frenados, 0 = pueden pasar


preparado()

int v = preparados.getAndInc()

if (v == n -1) // -1 porque empezo en 0 y el get me devuelve antes de suma, osea es el proceso n
    barrera.set(0)

while(barrera == 1){} // busy waiting !, asumimos que barrera es atomico para leerlo, sino getAndAdd(0)

critica()
```

a)
Pense que iba a ser peor con atomic...es argumentable, pero diria que la de semaforos es mas escalable, y a la larga debe ser lo que mas gente entiende y es mas facil
Supongo que esta dejo un poco mas a lugar la interpretacion asi suelto, y que si no lo estas usando siempre tendrias que tomarte un momento a pensarlo
b)
En ambos casos son atomicas, imagino que optimizadas por el hw..y ambos son como un llamado de funcion. No creo que haya mucha diferencia, quizas gane el semaforo por cuestion de ser algo mas usado y posiblemente optimizado que si yo voy y uso variables al bolero.
Importante, aca estamos usando busy waiting. Gana por goleada el semaforo que duerme a los procesos.
c)
Las variables atomicas requieren HW para garantizar que esa instruccion sea atomica
El semaforo tambien requiere que ciertas instrucciones sean atomicas, ademas se necesita que el sistema operativo provea las syscalls necesarias como el wait, poder poner procesos en ready cuando se llama a signal, etc