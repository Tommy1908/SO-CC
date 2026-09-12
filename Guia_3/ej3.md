Si hay 3 procesos y 1 mutex.
(la cola del wait?? se refiera dentro del signal usa una cola/ o lifo? o nada que ver)
Ejecuta el 1, ejecuta el 2 se va a la pila, ahora entra el 3ro. Termina el 1, sale el 3.
Entra a la pila el 1, termina el 3, entra a la pila el 3...
Hay starvation para el 2
