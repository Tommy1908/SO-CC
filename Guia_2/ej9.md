Considere los siguientes procesos:

```
Proceso | Ráfaga de CPU | Instante de llegada
P1      | 8             | 0
P2      | 8             | 5
P3      | 6             | 14
P4      | 5             | 15
```
a) Realizar un diagrama de Gantt para un algoritmo de scheduling round-robin con un quantum de 5 unidades de tiempo.


```
p1 0-5 | P1 5-8 | P2 8-13 | P2 13-16 | P3 16-21 | P3 21-23 | p4 23-28
P1 0-5 | P2 5-10 | P1 10-13 | P2 13-16 | P3 16-21 | P4 21-26 | P3 26-27
Todos los procesos llegan cuando ya empezo la ronda, entonces nunca se cambia de orden
Para que se vea algun cambio tendrian que durar mas los que ya estaban

P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   6-5=1               |   1-1   = 0
P2  |   16-6=10             |   10-10 = 0
P3  |   17-7=10             |   10-1  = 9
P4  |   27-8=19             |   19-10 = 9

TAT promedio = 40/4 = 10
WT  promedio = 18/4 = 4.5
```
