Para los procesos presentados en la siguiente tabla, realizar un gráfico de Gantt para cada uno de los algoritmos de scheduling indicados:

- FCFS.

- RR (quantum=10).

- SJF

```
Proceso | Ráfaga de CPU | Instante de llegada
P1      | 1             | 5
P2      | 10            | 6
P3      | 1             | 7
P4      | 10            | 8
```

## FCFS (First-Come, First-Served)

```
Idle 0-5 | P1 5-6 | P2 6-16 | P3 16-17 | P4 17-27

P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   6-5=1               |   1-1   = 0
P2  |   16-6=10             |   10-10 = 0
P3  |   17-7=10             |   10-1  = 9
P4  |   27-8=19             |   19-10 = 9

TAT promedio = 40/4 = 10
WT  promedio = 18/4 = 4.5
```

## RR Q10

Es exactamente igual a FCFS ya que el quantum es tan grande como la maxima rafaga y el orden es el mismo aca

```
Idle 0-5 | P1 5-6 | P2 6-16 | P3 16-17 | P4 17-27

P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   6-5=1               |   1-1   = 0
P2  |   16-6=10             |   10-10 = 0
P3  |   17-7=10             |   10-1  = 9
P4  |   27-8=19             |   19-10 = 9

TAT promedio = 40/4 = 10
WT  promedio = 18/4 = 4.5
```

## SJF

```
Idle 0-5 | P1 5-6 | P2 6-16 | P3 16-17 | P4 17-27

P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   6-5=1               |   1-1   = 0
P2  |   16-6=10             |   10-10 = 0
P3  |   17-7=10             |   10-1  = 9
P4  |   27-8=19             |   19-10 = 9

TAT promedio = 40/4 = 10
WT  promedio = 18/4 = 4.5
```

En pocas palabras los 3 son iguales, el RR porque el quantum es muy grande y SJF, por el orden, como es nonpreemtive, el de 10 (p2) le hacen efecto convoy al 1 (p3) que no llego a tiempo para ser evaluado por su longitud.

***

De yapa porque me eqivoque e hice SJF preemptive

## SRTF (Shortest Remaining Time First)

```
Idle 0-5 | P1 5-6 | P2 6-7 | P3 7-8 | P2 8-17 | P4 17-27

P   |   TAT (Fin - Llegada) |   WT (TAT-RAFAGA)
P1  |   6-5=1               |   1-1   = 0
P2  |   17-6=11             |   11-10 = 1
P3  |   8-7=1               |   1-1   = 0
P4  |   27-8=19             |   19-10 = 9

TAT promedio = 32/4 = 8
WT  promedio = 10/4 = 2.5
```
Este si es mas rapido