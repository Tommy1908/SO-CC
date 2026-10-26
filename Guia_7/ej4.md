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
