Se tiene un disco con capacidad de 128 GB, y bloques de 8 KB.
Supongamos un sistema de archivos similar a FAT, donde la tabla se ubica desde la posición 0, y cada entrada de la tabla es de 24 B.
a) ¿Qué tamaño ocupa la tabla?
128*10⁹ / 8*10³ = 16.000.000 entradas
16.000.000 * 24 = 384000000 bytes = 0.384GB
b) ¿Cuántos archivos de 10 MB es posible almacenar?
Si cada archivo necesita -> 10MB/8KB = 1250 entradas.
16.000.000/1250 = 12800 archivos posibles para almacenar.
---
Va devuelta porque me olvide la FAT. Que tambien ocupa un lugar en el disco, por lo tanto entradas de la FAT.
0.384GB / 8KB -> 48000 bloques (entradas)
(16.000.000 - 48000) / 1250 = 12761,6 -> 12761 archivos posibles para almacenar. 
c) Se sabe que un archivo comienza en el bloque 20. Dada la siguiente FAT, indicar el tamaño de dicho archivo.

Bloque     0  1 2  3 4 5 6 ... 20 21 22 ...
Siguiente EOF 2 23 4 5 0 7 ... 21 22 3 ...

20-> 21 -> 22 -> 3 -> 4 -> 5 -> 0.
7 bloques -> 7  * 8KB = 56KB.
Si bien es cierto que el tamaño real (usado) del archivo podria ser 48K< size(archivo) <= 56KB. Para el sistema hay 56KB ocupados