Sean P0, P1 y P2 tales que
P0 tiene ráfagas cortas de E/S a ciertos dispositivos.
P1 frecuentemente se bloquea leyendo de la red.
P2 tiene ráfagas prolongadas de alto consumo de CPU y luego de escritura a disco.

a) Para planificar estos procesos, ¿convendría usar un algoritmo de Round Robin? ¿convendría
usar uno de prioridades? Justifique su respuesta.
No hay indicios de necesidad de prioridad, y los primeros 2 parecen usar poco el cpu. Un round robbin entre todos funcionaria, ya que ponemos a cargar los primeros 2, y el p2 tiene el cpu mas tiempo luego porque cuando p0 y p1 se blooqueen quizas reciba mas quantums. De todas formas, si no fuera asi, aun se ejecutarian todos.

Otra alternativa, seria implementar por prioridad 0 a p0 y p1. Y p2 en una prioridad mas abajo, aprovecharia cuando p0 y p1 esten bloqueados por I/O. Si esto no pasara nunca ya que p0 y p1 se sincroniza (La cola de prioridad maxima tendria un round robin), para evitar starvation p2 sube luego de alguna cantidad de ciclos sin ser asignado, y se queda otra pequeña cantidad de ciclos para "recuperar" y vuelve a bajar
