a)

```
P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   3-0=3               |   3-3  = 0
P2  |   15-2=13             |   13-6 = 7
P3  |   8-4=4               |   4-4  = 0
P4  |   20-6=14             |   14-5 = 9
P5  |   10-8=2              |   2-2  = 0
TAT promedio = 36/5 = 7.2
WT  promedio = 16/5 = 3.2
```

b)
Se ve que cuando llega p2 en el instante 2, no se desaloja a p1, sino que termino su rafaga, le quedaba 1.
Luego p2 hace 1 de su rafaga, y es desalojado para p3 que es as corto. (5 en ese momento vs 4).
Sabemos que es preemptive entonces.
Por ese comportamiento y los siguientes donde siempre termina los mas cortos primeros conclusho que es SRTF (Shortest Remaining Time First) que es SJF verson preemptive
(P2 va antes de p4 porque llego antes y tenian el mismo tiempo restante)
