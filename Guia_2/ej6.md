Considerar el siguiente conjunto de procesos:

a) Dibujar los diagramas de Gantt para ilustrar la ejecución de estos procesos usando los algoritmos de scheduling FCFS, SJF, con prioridades sin desalojo (a menor el número, mayor la prioridad), round-robin (quantum de 1 unidad de tiempo, ordenados por el número de proceso).

b) ¿Cuál es el waiting time promedio y de turnaround promedio para cada algoritmo?

c) ¿Cuál de los algoritmos obtiene el menor waiting time promedio, y el menor turnaround?

Waiting time es la suma de los periodos en ready.
Turnaround es cuanto le toma a un proceso terminar de ejecutar (tiempo en ready + tiempo ejecutando en CPU + tiempo en I/O)

```
Proceso | Ráfaga de CPU | Prioridad
P1      | 10            | 3
P2      | 1             | 1
P3      | 2             | 3
P4      | 1             | 4
P5      | 5             | 2

Se supone que los procesos llegan en el orden P1, P2, P3, P4, P5 en el instante 0.
```

## FCFS (First-Come, First-Served)

```
P1 0-10 | P2 10-11 | P3 11-13 | P4 13-14 | P5 14-19

P   | Termina en..(TAT) |   WT (TAT-RAFAGA)
P1  |   10              |   10-10 = 0
P2  |   11              |   11-1  = 10
P3  |   13              |   13-2  = 11
P4  |   14              |   14-1  = 13
P5  |   19              |   19-5  = 14
TAT promedio = 67/5 = 13.4
WT  promedio = 48/5 = 9.6
```

## SJF (Shortest-Job-First)

```
P2 0-1 | P4 1-2 | P3 2-4 | P5 4-9 | P1 9-19

P   | Termina en..(TAT) |   WT (TAT-RAFAGA)
P1  |   19              |   19-10 = 9
P2  |   1               |   1-1   = 0
P3  |   4               |   4-2   = 2
P4  |   2               |   2-1   = 1
P5  |   9               |   9-5   = 4
TAT promedio = 35/5 = 7
WT  promedio = 16/5 = 3.2

```

## Con prioridades sin desalojo

En caso de empate, va el primero

```
P2 0-1 | P5 1-6 | P1 6-16 | P3 16-18 | P4 18-19

P   | Termina en..(TAT) |   WT (TAT-RAFAGA)
P1  |   16              |   16-10 = 6
P2  |   1               |   1-1   = 0
P3  |   18              |   18-2  = 16
P4  |   19              |   19-1  = 18
P5  |   6               |   6-5   = 1
TAT promedio = 60/5 = 12
WT  promedio = 41/5 = 8.2
```

## Round Robin quantum de 1, odenados por numero de proceso

```
P1 0-1 | P2 1-2 | P3 2-3 | P4 3-4 | P5 4-5 | P1 5-6 | P3 6-7 | P5 7-8 | P1 8-9 | P5 9-10 | P1 10-11 | P5 11-12 | P1 12-13 | P5 13-14 | P1 14-19 (solo queda el)

P   | Termina en..(TAT) |   WT (TAT-RAFAGA)
P1  |   19              |   19-10 = 9
P2  |   2               |   2-1   = 1
P3  |   7               |   7-2   = 5
P4  |   4               |   4-1   = 3
P5  |   14              |   14-5  = 9
TAT promedio = 46/5 = 9.2
WT  promedio = 27/5 = 5.4
```

c)
TAT promedio minimo: SJF 7
WT promedio minimo: SJF 3.2
