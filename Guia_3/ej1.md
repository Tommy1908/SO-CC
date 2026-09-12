1.  Se tienen dos procesos A y B que ejecutan concurrentemente. No se tiene información sobre cómo serán ejecutados por el scheduler. Para cada par A-B detallado a continuación, responder

Caso 1:
Podria salir de a 1 o 2, dependiendo si B se ejecuta antes que A o en medio de A

Caso 2:
Si primero ejecuta todo A, imprimira 0, 1, 2 ,3 y nunca 'a'

Si ejecuta primero A, hasta el y=1, imprime 1, luego ejecuta 'a', alguna cantidad de veces, luego ejecuta A, imprime 2, 3 luego va B devuelta, imprime 'a'. Si en el medio entre y=0 y antes de y=1 le toca a B no imprime 'a'. Pero con esto ya muestra que la salida podria ser diferente.
