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

while(barrera = 1){} // busy waiting !, asumimos que barrera es atomico para leerlo, sino getAndAdd(0)

critica()
```
