Ejercicio 4 ⋆
Suponer que se cuenta con un sistema de archivos basado en inodos como ext2 y bloques de 4 KB.
a) Si se tiene un archivo de 40 KB, ¿cuántos bloques es necesario leer para procesarlo completamente?
b) ¿Cuántos bloques es necesario leer si el archivo tiene 80 KB?

Nota: Venia haciendo con potencias de 10, vamos a hacerlo con potencias de 2 ahora. Es decir 80KB 80*10³B antes, ahora 80Kib = 80*1024bits = 80\*2^10.
Nose si es mejor...tampoco se cual esperan que usemos la verdad

a)
40kb = 40 _ 2^10
4kb = 4 _ 2^10
40/4 -> necesitamos 10 bloques.
b)...20(?)

Ah okey como ext2 se supone que me dice 12 directos, 1 simple, 1 doble, 1 triple.
a) Sigue siendo 10
b) 21, porque 10 directos, + 1 por bloque simple, y leo los 10 que me faltaban. 21 bloques totales, 20 de info 1 de punteros
