Ejercicio 5
Considerando un sistema ext2 (12 entradas directas, 3 indirectas, 1 doble indirecta y 1 triple indirecta) y un sistema basado en FAT:
¿Cuántos bloques de disco se deben acceder para leer los siguientes bloques de un archivo?
i) 1, 2, 3, 5, 7, 11, 13, 17, 23.
ii) 1, 2, 3, 4, 5, 6, 10001.
iii) 13, 10000, 1000000.
iv) 14, 15...50.

Asumir que si un bloque se lee dos veces, se va a buscar una sola vez al disco, que tanto la FAT como el inodo del archivo correspondiente ya están cargados en memoria, y que se conoce el número de bloque inicial (en FAT) y el número de inodo (en ext2).

i)
FAT : 9
ext2: 6 + (1 + 3) = 10

ii) Voy a suponer 4kb cada bloque y 4b (32-bits)
4kb -> 1024 punteros por bloque. Primer bloque indireccion [1024] (hay 3). Segundo [1024*1024 = 1048576]

FAT: 7
ext2: 6 + (1 (acceso a indireccion doble) + 1 (acceso de indireccion simple proveniente de doble) + 1) = 9

iii)
Cada bloque de indireccon doble cubre 1024, por lo tanto no van a acceder al mismo
FAT: 3
ext2: (1 indirecto simple + 1) + 1 (acceso a indireccion doble) + (1 (acceso a indireccion simple proveniente de doble) + 1)\*2 = 7

iv)
FAT: 37
ext2: (1 indirecto simple) + 37 = 38
