Ejercicio 7
Un esquema de protección implementado por algunos sistemas operativos consiste en colocar el stack del proceso en una posición de memoria al azar al iniciar (stack randomization).
Indique cuáles de las siguientes estrategias de ataque a una función con problemas de seguridad se ven fuertemente afectadas por esta protección, y explique por qué:

a) Escribir el valor de retorno de una función utilizando un buffer overflow sobre un buffer en stack dentro de dicha función.
No le afecta, porque el ret va a estar en igual offset respecto a mi buffer, se mueve el stack entero, y el ret guarda la posicion absoluta de como regresar.
b) Utilizar el control del valor de retorno de una función para saltar a código externo introducido en un buffer en stack controlado por el usuario.
Okey, esto es de otra funcion, cambiar el ret y saltar a lo que yo escribi en el buffer para que me ejecute esto. Claro esto no se podria con facilidad, ya que el stack cambiaria cada vez (cada proceso), el punto es que quizas podrias acertar una vez, pero sin duda es mas dificil.
c) Utilizar el control del valor de retorno de una función para ejecutar una syscall particular (por ejemplo read) que fue usada en otra parte del programa.
No tiene nada que ver, si puedo cambiar el retorno de una funcion, para saltar a una parte vieja del codigo, el codigo no esta en el stack. Osea que si puedo hacer como en el inciso a, o por alguna razon puedo elegir el retorno, y se donde esta el codigo, que el stack se mueva me da igual.