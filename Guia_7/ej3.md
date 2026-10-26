Ejercicio 3
Si el usuario tiene control sobre la entrada estándar, el siguiente código tiene problemas de seguridad.

```c
void saludo(void) {
    char nombre[80];
    printf("Ingrese su nombre: ");
    gets(nombre);
    printf("Hola %s!\n", nombre);
}
```

Se considera como un problema de seguridad que un usuario atacante pueda realizar operaciones que no le eran permitidas, ya sea tanto acceder a datos privados, ejecutar código propio en otro programa o inhabilitar un servicio. Determinar:
a) ¿Dónde se introduce el problema de seguridad?
Gets lee y escribe hasta el EOF. Al no haber restriccion de tamaño, puedo escribir mas de 80 caracteres y pasarme del limite del buffer "nombre", escribiendo en otro lugar.
b) ¿Qué datos del programa (registros, variables, direcciones de memoria) pueden ser controladas
por el usuario?


En principio solo controla lo que se puede escribir dentro de nombre, esos 80bytes. Luego si hace un buffer overflow, puede escribir en otras partes. (Sobreescribir si se quiere)

```
Direcciones mayores ↑

0x1058 — Return address de saludo
0x1050 — RBP guardado

0x104F — nombre[79]
    gets escribe hacia arriba ↑
0x1000 — nombre[0]

Direcciones menores ↓
Stack crece ↓
```
Entonces podria escribir sobre el RBP yo el RET address de saludo para llevarme a otra posicion

c) ¿Es posible sobreescribir la dirección de retorno a la que vuelve la llamada de alguna de las funciones printf o gets?
Diria que esas no, porque tendria que romper como dentro de esa funcion, nose como seria.
Yo estoy en saludo, me guardo en el stack mi pos actual, y me voy a print, hago cosas, y a menos que DENTRO de print sobreescriba, no puedo, y en el gets diria que tampoco, lo que me rompe aca es el efecto del gets, el gets me permite romper, el ret de saludo, y las posiciones de gets no estan justo debajo de mi ret.
Osea el ret de gets esta mas abajo que nombre, porque el stack crece hacia abajo, y gets no se rompe a si mismo en principio. asi que no.Gets rompe escribiendo en el buffer que esta mas arriba
RET SALUDO
NOMBRE
RET GETS
Gets escribe desde nombre hacia ret saludo
d) ¿Se soluciona el problema de seguridad si se elimina el segundo printf?
No, se soluciona limitando la escritura al tamaño del buffer, por ejemplo con fgets











Proceso A
STACK
|
V

A
|
HEAP
DATA
CODIGO

Dentro del stack

-                | crece
  MAIN() v
  SALUDO
  rbp anterior
  nombre[80] (saludo)

*
