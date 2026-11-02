Ejercicio 6
Considerar la siguiente ruta a un archivo: /home/aprobar.txt.
a) En un sistema basado en FAT, ¿cuántos bloques de disco se deben acceder para llegar a los bloques de datos de este archivo?

Tengo que entrar a la tabla, buscar home el directorio, para ello tengo que recorrer todos los bloques de root en peor caso (dentro de cada uno). Luego recorrer todos los bloques de home en peor caso hasta encontrar txt.
O(|ROOT|+|HOME|)

b) En un sistema basado en ext2, se quiere leer el archivo /pepe.txt, que es un enlace simbólico al archivo mencionado arriba. El enlace no está cargado en memoria. 
¿Cuántos bloques de disco se deben acceder para llegar a los bloques de datos del archivo referenciado?

Un enlace simbolico es un archivo, que tiene un inodo diferente al del archivo objetivo, que guarda la ruta al inodo.
Como esta en root pepe, lo buscamos, potencialmente buscando en todo root (*). y hacemos el acceso a pepe. Ahi obtenemos la ruta nueva en texto, y buscamos.
Luego una vez mas en root (no lo tengo que buscar root siempre es inodo 2), busco home, (*) puede tomar tantos accesos como todos los bloques que haya en root. Maximo 12 + 1024 *3 + 1024^2 + 1024^3 en ext2 con punteros 32bits (4b) y bloques de 4kb
Por ultimo una vez home lo mismo para encontrar el txt.
Mejor caso son 7 accesos. Entrar a root(1) y cargar el primero(1), encontrar pepe y cargar el inodo de pepe(1) y buscar su contenido para el link en texeto(1). buscar en root igual (asumo que como solo de root lei uno si queda cargado) busco home y esta, cargo el inodo de home (1) y entro al primero(1) y esta aprobar.txt (1)
Peor caso O((12 + 1024 + 1024^2 + 1024^3) * 3) cantidad de bloques a recorrer
Podria pasar que root quedo en memoria, eso nos ahorraria un acceso al disco. No se si podemos asumirlo

$\underbrace{12+3(1024)+1024^2+1024^3}_{\text{bloques de datos}}+\underbrace{3+(1+1024)+(1+1024+1024^2)}_{\text{bloques auxiliares}} * 3$ cantidad de accesos a bloques en peor caso
Toda la sumas son recorriendo los bloques de entrys, tratando de encontrar el directorio/archivo correcto.
Suma el acceso a cada inodo cuando lo encontrar. Root + pepe + Root + home + aprobar
+ 1 de entrar el primer acceso dentro del enlace simbolico para encontrar el path
