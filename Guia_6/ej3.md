Ejercicio 3
Un sistema de archivos de UNIX, similar a ext2, tiene bloques de tamaño 4 KB y el direccionamiento a bloques de disco (LBA) es de 8 B.
A su vez, cada inodo cuenta con 5 entradas directas, dos indirectas y una doblemente indirecta.

a) ¿Cuál es el tamaño máximo de archivo que soporta?

Cada bloque es de 4KB. Cada puntero es de 8bytes.
Con 5 entradas directas, tenemos 5 puntos, a 5 bloques por lo tanto 4*5kb por parte de las entradas directas
Con 2 indirectas, cada entrada directa nos redirige a un bloque de direcciones. Eso nos permite tener 4kb/8b = 500 entradas por cada bloque. En total 1000 bloques mas.
Por ultimo la doblemente indirecta nos permite tener 500 bloques con direcciones por lo tanto 500*500 bloques extra (250000 bloques)
En total 5+2\*500+500^2 bloques y por lo tanto 251005 bloques que son 1004020KB

b) Si el 50 % del espacio en disco está ocupado por archivos de 2 KB, el 25 % por archivos de 4 KB y el 25 % restante por archivos de 8 KB,
¿qué porcentaje del espacio en disco está siendo desperdiciado? (Considerar sólo el espacio utilizado en los bloques de datos).

Los de 4kb y los de 8kb no desperdician nada, los de 2kb desperdician el 50% de cada bloque. Y los de 2KB son el 50%, por lo tanto desperdician el 25% del disco.

c) ¿Cuántos bloques es necesario acceder para procesar completo un archivo de 5 MB?

5000kb/4kb = 1250 bloques totales. Como de 1250 (asumiendo que ya tenemos cargado el inodo y que cada bloque axuliar se carga solo 1 vez), 5 solo son directos, tenemos que leer 2 bloques de direccion simple. Hasta ahi leimos 1250-5-1000. Para los que quedan leemos otro bloque extra para la indirecta doble. De esta, hay que solo leer 1 puntero mas pues nos faltan 245 bloques que con 1 bloque de punteros cubrimos. Asi que sumamos 2 lecturas mas.
Quedan 5 accesos a la directa. (1 al bloque simple + 500 de cada puntero dentro) * 2 + (1 al bloque doble + 1 bloque del indirecto simple del doble + 245 que faltaban) = 5 + 1002 + 247 =  1254