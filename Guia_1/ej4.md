![alt text](image.png)

New -> Ready: Crear todo y ponerla en el scheduler como lista
Ready -> Running: Sheduler le dio el turno, se carga su PCB
Running -> Ready: Se acabo el Quantum asignado, o hay una interrupcion, se guarda todo
Running -> Blocked: Pide algo que no tiene y queda bloqueado esperando
Blocked -> Ready: C0nsigui el dato o se libero el recurso necesario y esta libre (Tendria que notificarse con una interrupcion ya que no va a ser ejecutado en la mayoria de implementacion si esta blocked)
Running -> Terminated: El proceso termino