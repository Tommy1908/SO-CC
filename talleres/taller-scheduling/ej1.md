1. Para familiarizarse con el simulador, generar dos problemas con datos aleatorios (notar que es una posibilidad nativa del simulador) deshabilitando E/S.

a) ¿Qué diferencias se observan entre los dos problemas generados?

b) Hacer el ejercicio de representar al menos uno de los casos mediante un diagrama de Gantt.

c) ¿Qué características del algoritmo de Multilevel Feedback Queue se pueden reconocer a partir de lo observado?
Nota: Te podés ayudar limitando la duración de las tareas a un valor razonable y corto (entre 20 y 30ms por ejemplo).

```
❯ python mlfq.py -c -j 3 -m 25 -s 19 -q 5
Here is the list of inputs:
OPTIONS jobs 3
OPTIONS queues 3
OPTIONS allotments for queue  2 is   1
OPTIONS quantum length for queue  2 is   5
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is   5
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is   5
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime  17 - ioFreq   8
  Job  1: startTime   0 - runTime  13 - ioFreq   5
  Job  2: startTime   0 - runTime  10 - ioFreq   9


Execution Trace:

[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] JOB BEGINS by JOB 2
[ time 0 ] Run JOB 0 at PRIORITY 2 [ TICKS 4 ALLOT 1 TIME 16 (of 17) ]
[ time 1 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 15 (of 17) ]
[ time 2 ] Run JOB 0 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 14 (of 17) ]
[ time 3 ] Run JOB 0 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 13 (of 17) ]
[ time 4 ] Run JOB 0 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 12 (of 17) ]
[ time 5 ] Run JOB 1 at PRIORITY 2 [ TICKS 4 ALLOT 1 TIME 12 (of 13) ]
[ time 6 ] Run JOB 1 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 11 (of 13) ]
[ time 7 ] Run JOB 1 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 10 (of 13) ]
[ time 8 ] Run JOB 1 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 9 (of 13) ]
[ time 9 ] Run JOB 1 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 8 (of 13) ]
[ time 10 ] IO_START by JOB 1
IO DONE
[ time 10 ] Run JOB 2 at PRIORITY 2 [ TICKS 4 ALLOT 1 TIME 9 (of 10) ]
[ time 11 ] Run JOB 2 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 8 (of 10) ]
[ time 12 ] Run JOB 2 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 7 (of 10) ]
[ time 13 ] Run JOB 2 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 6 (of 10) ]
[ time 14 ] Run JOB 2 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 5 (of 10) ]
[ time 15 ] IO_DONE by JOB 1
[ time 15 ] Run JOB 0 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 11 (of 17) ]
[ time 16 ] Run JOB 0 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 10 (of 17) ]
[ time 17 ] Run JOB 0 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 9 (of 17) ]
[ time 18 ] IO_START by JOB 0
IO DONE
[ time 18 ] Run JOB 2 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 4 (of 10) ]
[ time 19 ] Run JOB 2 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 3 (of 10) ]
[ time 20 ] Run JOB 2 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 2 (of 10) ]
[ time 21 ] Run JOB 2 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 1 (of 10) ]
[ time 22 ] IO_START by JOB 2
IO DONE
[ time 22 ] Run JOB 1 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 7 (of 13) ]
[ time 23 ] IO_DONE by JOB 0
[ time 23 ] Run JOB 1 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 6 (of 13) ]
[ time 24 ] Run JOB 1 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 5 (of 13) ]
[ time 25 ] Run JOB 1 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 4 (of 13) ]
[ time 26 ] Run JOB 1 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 3 (of 13) ]
[ time 27 ] IO_START by JOB 1
IO DONE
[ time 27 ] IO_DONE by JOB 2
[ time 27 ] Run JOB 0 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 8 (of 17) ]
[ time 28 ] Run JOB 0 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 7 (of 17) ]
[ time 29 ] Run JOB 2 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 0 (of 10) ]
[ time 30 ] FINISHED JOB 2
[ time 30 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 6 (of 17) ]
[ time 31 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 5 (of 17) ]
[ time 32 ] IO_DONE by JOB 1
[ time 32 ] Run JOB 0 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 4 (of 17) ]
[ time 33 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 3 (of 17) ]
[ time 34 ] Run JOB 0 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 2 (of 17) ]
[ time 35 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 2 (of 13) ]
[ time 36 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 1 (of 13) ]
[ time 37 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 0 (of 13) ]
[ time 38 ] FINISHED JOB 1
[ time 38 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 1 (of 17) ]
[ time 39 ] IO_START by JOB 0
IO DONE
[ time 39 ] IDLE
[ time 40 ] IDLE
[ time 41 ] IDLE
[ time 42 ] IDLE
[ time 43 ] IDLE
[ time 44 ] IO_DONE by JOB 0
[ time 44 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 0 (of 17) ]
[ time 45 ] FINISHED JOB 0

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  45
  Job  1: startTime   0 - response   5 - turnaround  38
  Job  2: startTime   0 - response  10 - turnaround  30

  Avg  2: startTime n/a - response 5.00 - turnaround 37.67


```

C) Se ve que el Job 1 paso de priority 2 a priority 1 porque agoto la cantidad de quantums.
Con esta config no suben.

2.

3.
En caso que programa maliciso haga un I/O que dure lo minimo posible o falso, evita agotar su quantum, y quedar como un proceso I/O que consume poco y asi no bajar su prioridad.

4.
Caso basico

```
❯ python mlfq.py -c --jlist 0,30,0:0,8,2:10,12,0 -q 7

Job List:
  Job  0: startTime   0 - runTime  30 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   2
  Job  2: startTime  10 - runTime  12 - ioFreq   0

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  50
  Job  1: startTime   0 - response   7 - turnaround  34
  Job  2: startTime  10 - response   0 - turnaround  23

  Avg  2: startTime n/a - response 2.33 - turnaround 35.67
```

Bajamos el quantum, que le da cpu al proceso E/S mas seguido, y ademas -S la flag, hace que no baje de prioridad los de E/S.
-a para que los otros bajen de prioridad e I para volver primero luego del E/S

```
❯ python mlfq.py -c --jlist 0,30,0:0,8,2:10,12,0 -q 2 -S -a 1 -I

Job List:
  Job  0: startTime   0 - runTime  30 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   2
  Job  2: startTime  10 - runTime  12 - ioFreq   0

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  50
  Job  1: startTime   0 - response   2 - turnaround  27
  Job  2: startTime  10 - response   0 - turnaround  24

  Avg  2: startTime n/a - response 0.67 - turnaround 33.67


```

Aca, le ponemos un quantum grande en particular 30 que es el del que mas tarda para asegurarnos que sea el mas efectivo ya que entra y termina todo a la primera. Empeoran mucho los otros

```
❯ python mlfq.py -c --jlist 0,30,0:0,8,2:10,12,0 -q 30

Job List:
  Job  0: startTime   0 - runTime  30 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   2
  Job  2: startTime  10 - runTime  12 - ioFreq   0
ww
Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  30
  Job  1: startTime   0 - response  30 - turnaround  60
  Job  2: startTime  10 - response  22 - turnaround  34

  Avg  2: startTime n/a - response 17.33 - turnaround 41.33

```

Si elegimos algo mas moderado como 15, ambos el mediano y el pesado se benefician un poco.

```
❯ python mlfq.py -c --jlist 0,30,0:0,8,2:10,12,0 -q 15

Job List:
  Job  0: startTime   0 - runTime  30 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   2
  Job  2: startTime  10 - runTime  12 - ioFreq   0

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  50
  Job  1: startTime   0 - response  15 - turnaround  45
  Job  2: startTime  10 - response   7 - turnaround  19

  Avg  2: startTime n/a - response 7.33 - turnaround 38.00

```