4.Starvation???
a. Round Robin
No, nunca en principio.
Si bien podria ser mucho lo que tarde en tocarle, esta asegurado que le toca 1 vez por ciclo (ronda)

b. Por prioridad
Si, una prioridad mas baja podria sufrir de inanicion si la prioridad mas alta siempre tiene un ready

c. SJF (Shortest-Job-Firs)
Si, si siempre aparecen procesos cortos, uno mucho mas largo no se ejecutaria
Se mide por lo que va a durar el proximo burst, o el realidad burst promedio ya que es por procesos anteriores que se sabe. Es sin contar el I/O.

d.SRTF(Shortest-Remaining-Time-Firt)
Analogo al SJF, si llega uno cortisimo simepre, uno largo ni la ve.

e. FIFO
Nunca, first in first out, implica una cola.

F. Colas de Multinivel
Si, una prioridad mas baja quedaria marginada si una de mayor prioridad siempre tiene ready.

G. Colas de multinivel con feedback (agin)
Justamente esta no, ya que el agin indica que si un proceso hace mucho no recibe el procesador, es decir empiza a sufrir starving, lo subimos de nivel, esto podria llevarle al extremo de estar en maxima prioridad, haciendo que se ejecute eventualmente.
Es dinamico, los procesos que empiezan a gastar mas cpu y agotar todo el quantum bajan y los que menos y se vuelven mas cortitos suben. Y ademas los aged suben, y se comportan como uno nuevo, osea puede volver a subir o bajar bajo el analisis anterior.
