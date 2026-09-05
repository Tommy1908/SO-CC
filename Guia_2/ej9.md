Considere los siguientes procesos:

```
Proceso | Ráfaga de CPU | Instante de llegada
P1      | 8             | 0
P2      | 8             | 5
P3      | 6             | 14
P4      | 5             | 15
```

a) Realizar un diagrama de Gantt para un algoritmo de scheduling round-robin con un quantum de 5 unidades de tiempo.
b) Realizar un diagrama de Gantt para un algoritmo tipo shortest remaining time first
c) Calcular el tiempo de turnaround promedio en ambos casos

## RR Q5

```
P1 0-5 | P2 5-10 | P1 10-13 | P2 13-16 | P3 16-21 | P4 21-26 | P3 26-27

P   |   TAT (Fin - Llegada) |
P1  |   13-0 =13            |
P2  |   16-5 =11            |
P3  |   27-14=13            |
P4  |   26-15=11            |

TAT promedio = 48/4 = 12
```

## SRTF

```
P1 0-8 | P2 8-16 | P4 16-21 | P3 21-27

P   |   TAT (Fin - Llegada) |
P1  |   8-0  =8             |
P2  |   16-5 =11            |
P3  |   27-14=13            |
P4  |   21-15=6             |

TAT promedio = 38/4 = 9.5
```

d) A pesar de que uno de los dos casos tiene un tiempo de turnaround promedio mucho menor,
explicar por qué en algunos contextos podría tener sentido utilizar la otra política. Para esto considere distintos tipos de procesos: real time, interactivos, batch, etc

Para procesos interactivos lo mejor seria el round robin porque permite dar la sensacion de fluides y que todo pasa al mismo tiempo, mostrando un poco de cada uno.
"la métrica más importante no es el Turnaround Time (cuánto tarda en terminar), sino el Tiempo de Respuesta (Response Time), que es cuánto tarda el sistema en empezar a reaccionar a una acción del usuario (ej. mover el mouse, teclear)." Calculo que tambien importa que te lo devuelvan, no solo la primera vez...pero qcyo cortesia de gemini

De real time todavia no hablamos...pero en teoria creo q ninguno seria super optimo.

Para procesos batch el SRTF podria ir perfecto porque nois permitiria tener la mayor cantidad de procesos lo antes posible, sin importar el orden. Quizas agregarle algo de agin para evitar starvation.
