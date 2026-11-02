Ejercicio 8
Se tiene un disco rígido de 16 GB de espacio con sectores de 1 KB. Se desea dar formato al disco usando un sistema de archivos de uso específico llamado HashFS, basado en FAT.
La idea es que no existen directorios ni archivos. Dado un path, se calcula el hash del nombre y éste indica cuál es el archivo buscado.
En resumen, este sistema de archivo cuentas con dos tablas:

- Una única FAT que guarda las entradas correspondientes al próximo bloque, indicando el final de un archivo cuando estos valores coinciden.
- Una única tabla de hash que contiene, para cada hash posible, el identificador del bloque inicial y el tamaño en bytes del archivo correspondiente a dicho hash.

La novedad es que este sistema de archivos permite configurar los siguientes elementos:

- Tamaño del bloque: 2, 4 u 8 sectores.
- Tamaño de identificadores de bloque: 8, 16, 24 o 32 bits.
- Tamaño del hash: 8, 16, 24 o 32 bits.

a) Suponiendo que se configura con 2 sectores por bloque, identificadores de bloque de 24 bits, y hash de 16 bits.
¿Cuál es el tamaño que ocupa la FAT? ¿Cuál es el tamaño de la tabla de archivos? ¿Cuál es el espacio que queda en disco para archivos?

Disco: 16 GiB.
Sector: 1 KiB.
Bloque: 2 sectores = 2 KiB.
Identificador de bloque: 24 bits = 3 B.
Hash: 16 bits.

La fat tiene el tamaño de la cantidad de bloques. min(16GB/ 2KB o 2 ^24),resultla = 2^23
Hay 2^23 bloques fisicos, y los identificadores permiten representar 2^24 valores.
Cada entrada ocupa 3B asi que la fat ocupa 2^23 * 3B = 24MiB

Y la de archivos 2^16 entradas, cada entrada tiene un identificador de bloque de 24 bits, y el size. Depende de como se guarda, si se limita a 4bytes, u 5b que es el maximo tamaño posible
Cada entrada ocupa entonces 3 + 5. 2^16 * 8b = 512KiB.

b) Sabiendo que se desea maximizar la cantidad de archivos que el sistema soporta y que, además, en promedio los archivos tendrán un tamaño de 1 KB, 
¿cuál sería la configuración óptima del sistema de archivos? Justificar.

c) ¿Cómo lo configuraría si el promedio de tamaño de archivos es de 16 MB? Justificar.
