Ejercicio 7
Una compañía que fabrica discos rígidos decide, como parte de cierta estrategia comercial, emprender la creación de un nuevo filesystem. 
Durante la fase de diseño preliminar el equipo de ingeniería a cargo del proyecto discute acaloradamente la conveniencia de adoptar un enfoque inspirado en FAT o la de uno basado en inodos. Indicar cuál de las dos opciones recomendaría, y por qué, para cada uno de los siguientes requerimientos:

1. Es importante que puedan crearse enlaces simbólicos.
ext2? vimos la creacion de enlaces duros y simbolicos sobre inodos. Pero no veo imposible hacerlo en fat. Punto para ext2 por ser mas directo supongo

2. Es importante que la cantidad de sectores utilizados para guardar estructuras auxiliares sea acotada, independientemente del tamaño del disco.
FAT tiene la tabla y crece segun el disco. Es cierto que ext2 esta repleto de estructuritas auxiliares, es mas complejo. Pero si hay menos archivos hay menos inodos. Podriamos decir que hay mas boilerplate para inodos.
Pero si haces modificaciones podrias hacer algo mas dinamico quizas que ocupe menos.

3. Es importante que el tamaño máximo de archivo sólo esté limitado por el tamaño del disco.
Entonces FAT. Es disco-lo que ocupe la tabla. Los inodos dependiendo como lo hagas van a tener un limite u otro, pero una vez establedecido el file system, si agregas mas disco no podes hacer un archivo de ese tamaño

4. Es importante que la cantidad de memoria principal ocupada por estructuras del filesystem en un instante dado sea (a lo sumo) lineal en la cantidad de archivos abiertos en ese momento.
Ext2

Nose si pide algo para las 4 restricciones a la vez..pero seria medio imposible.