Ejercicio 1
Suponer una computadora cuyo disco se accede sin memoria cache y un sistema de archivos FAT. 
Además, en este sistema, la FAT no queda almacenada en la memoria (recordar que lo normal es que la FAT se cargue en memoria).
¿Cuántos accesos al disco son necesarios para llegar hasta el último bloque de un archivo de N bloques?

Si la tabla FAT no esta memoria, y esta en un lugar del disco, asumo que se que archivo quiero ir y que ya se donde empieza.
1. Encontrar donde empieza mi archivo (para obtener si sigue o si termina ahi) -> acceder a la fat en el disco, en la posicion de donde empieza mi archivo
2. Encontrar acceder a la direccion que obtuvimos antes. -> acceder a la fat en el disco, en la posicion obtenida antes
...
N. Entrar y encontrar un eof. -> acceder a la fat en el disco, en la posicion obtenida antes
Si sabias que tenias N bloques, podrias haber frenado en N-1, nunca vez el EOF, pero sabes que no habia mas bloques.
Si ademas de hacer esto quisieras ir accedediendo al archivo, seria 2N, un acesso al bloque por cada direccion que encontramos.