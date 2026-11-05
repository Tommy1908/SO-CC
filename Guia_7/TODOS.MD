En muchos sistemas las contraseñas de los usuarios no se almacenan, sino que se almacena solo el
resultado de una función de hash. Suponiendo que la función de hash utilizada entrega valores de 64
bits y que los resultados se encuentran bien distribuidos, responder

a) ¿Cómo puede verificar el sistema si un usuario ingresó su contraseña correctamente, si no la
tiene almacenada en el sistema?
Tiene que aplicar la misma funcion de hash y verificar que sean iguales.

b) Si bien existen varias contraseñas que entregan el mismo valor de hash (colisión), ¿qué probabilidad hay de acertar el valor de hash almacenado?
Si estan bien distribuidos 1/2^64

c) ¿En cuántos años se puede tener un 50 % de probabilidad de haber acertado al valor de hash
almacenado, dado que pueden probarse mil millones de contraseñas por segundo?
Hay 2^64 hashes.
Para tener el 50% de probabilidad, tendrias que ya haber probado la mitad. 2^64/2 = 2^63.
2^63/10^9 (mil millones) -> 292 años

d) Conocer el valor de hash de una contraseña no implica conocer una contraseña que genere ese
valor. Suponer que se pueden probar contraseñas a la misma velocidad que en el ítem anterior,
pero nos informan que la contraseña es de, a lo sumo, 6 caracteres, y que estos solo son letras
minúsculas o dígitos. ¿Cuánto tiempo hay que esperar para averiguar la clave?
Si podes probar mil millones por segundo, (y no te bloquearan lo cual nunca entiendo como anda) tenes que hay 26 letras minusculas (sin ñ), y 10 digitos. Por cada slot hay 36 posibles combinaciones con repetidos. 36^6 es si es 6 digitos.
Hay 36+36²+36³+..+36⁶ contraseñas posibles. 2.238.976.116.
2.23 segs medio la cuenta

Ejercicio 2
Los sistemas de autenticación remota, donde el usuario se debe autenticar a través de un canal inseguro (que puede ser visto por terceros), como por ejemplo POP3, a menudo no transmiten la contraseña en el proceso de autenticación para evitar que ésta sea interceptada.

a) ¿Sería seguro autenticar al usuario enviando por el canal (desde el usuario hasta el sistema) el
hash de la contraseña? ¿A qué ataque es vulnerable este esquema?
Si envia el hash en el medio es vulnerable a Man in the middle, copia el hash y lo envia el (como paquete imagino). Aclara que se llama Replay Attack
b) Un esquema Challenge-Response basado en una función de hash opera de la siguiente manera:
El sistema envía una cadena seed elegida al azar.
El usuario responde con el resultado de hash(seed + contraseña).
El sistema hace la misma cuenta de su lado y verifica el resultado.
Si un atacante captura toda esta conversación, ¿sería capaz de realizar un ataque de fuerza
bruta sobre la contraseña sin realizar ninguna interacción con el servidor (ataque offline)?
Si, porque sabe la seed y sabe el resultado
seed + pass = hash_objetenido
Tiene que ir probando hasta adivinar pass

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

Ejercicio 4 ⋆
El siguiente es un sistema de login que valida los datos contra una base de datos.

```c
struct credential {
    char name[32];
    char pass[32];
}
bool login(void) {
    char realpass[32];
    struct credential user;
    // Pregunta el usuario
    printf("User: ");
    fgets(user.name, sizeof(user), stdin);
    // Obtiene la contraseña real desde la base de datos y lo guarda en realpass
    db_get_pass_for_user(user.name, realpass, sizeof(realpass));
    // Pregunta la contraseña
    printf("Pass: ");
    fgets(user.pass, sizeof(user), stdin);
    return strncmp(user.pass, realpass, sizeof(realpass)-1) == 0;
    // True si user.pass == realpass
}
```

Suponiendo que la función db_get_pass_for_user() es totalmente correcta y no escribe fuera de
realpass():

a) Hacer un diagrama de cómo quedan ubicados los datos en la pila, indicando claramente en qué sentido crece la pila y las direcciones de memoria.
Explicar, sobre este diagrama, de qué datos (posiciones de memoria, buffers, etc.) tiene control el usuario a través de la función fgets().

```
ret login
stack:
realpass
user [64[pass]32[name]0] // o al reves, no estoy seguro, esta forma seria la que nos permite romper y creo que podemos elegir la que preferimos para el ejercicio dijieron
```

Un usuario puede escribir sobre pass y name. El fgets se sizeof user, no size off name, le regalan 32 bits. Lo mismo con la contraseña. Puede pisar realpass con un buffer overflow, sobre password, pisando realpassword y hacer que la comparacion de igual

b) Indicar un valor de la entrada, que pueda colocar el usuario, para loguearse como admin sin
conocer la contraseña de este.
admin
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
strcmp compara los 32 primeros. No hay problema con el \0


EJ6
Considerando que el usuario controla el valor del parámetro f, analizar el siguiente código de la función signo.

```c
#define NEGATIVO 1
#define CERO 2
#define POSITIVO 3

int signo(float f) {
    if (f < 0.0) return NEGATIVO;
    if (f == 0.0) return CERO;
    if (f > 0.0) return POSITIVO;

    assert(false && "Por aca no paso nunca");
    return 0; // Si no pongo esto el compilador se queja =(
}
```

a) ¿El usuario tiene alguna forma de que se ejecute el assert()? 
Pista: Piense en el formato IEEE-754.
Osea, creo que seria imposible no quedar en ninguno de los if, son exhaustivos, pero podriamos hacer que el resultado este mal.
IEEE-754 -> Signo | Exponente | Mantissa
Bueno, falso, si podia romperse. Existe "nan" que no cae en ninguno de los 3, asi que es necesario el assert.

b) En las versiones “release” los assert suelen ignorarse (opción de compilación). ¿Sería seguro utilizar la función signo sobre un dato del usuario y esperar como resultado alguno de los valores 1, 2 o 3?
No porque NaN romperia, y daria 0