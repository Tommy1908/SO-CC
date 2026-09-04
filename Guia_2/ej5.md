Considere una modificación a round-robin en la que un mismo proceso puede estar encolado varias veces en la lista de procesos ready.
Por ejemplo, en un RR normal se tendrían en la cola ready a P1,P2,P3,P4, con esta modificación se podría tener P1,P1,P2,P1,P3,P1,P4.

a. Si garantiza que el orden es unico, o que no pueden encolarse nuevos procesos al principio, en principio aun se garantiza que no hay starvation.
Sin embargo esta modificacion va a dar mas quantums a los procesos repetidos que a los que no, seria mejor para los repetidos, peor para los no repetidos (injusto?)
b.
(Si ponemos repetidos procesos de intensivo I/O, que por lo general no terminan su quantum, podriamos dar mas ilusion de interactividad. Es decir, procesos que son lentos mezclados con procesos rapidos, podriamos repetir mas veces los rapidos, si fuera al reves, seria incluso peor la interactividad.)
En realidad no, porque cuando el proceso se bloquee va a salir del ready.
Entonces lo que pasa en realidad es que benficia a proceso mas intensivos de cpu por darles mas quantums.
c.
(Usar una cola de prioridad para intensivos en I/O con round robin y otra cola para los mas lentos. Si tiene que ser si o si, round robins's podria haber 2 colas. Una de procesos que no completan el quantum general, y otra que si, y la que es mas rapida correrla 2 veces. Rapida-Lenta-Rapida-Rapida-Lenta-Rapida...)Basado en mi respuesta inicial
Para replicar el efecto, de lo que comente abajo, seria un round robin, pero que tengan un quantum mas grande, en vez de todos iguales. Asi los que estaban 3 veces, ahora tienen un quantum 3 veces mas grande.
