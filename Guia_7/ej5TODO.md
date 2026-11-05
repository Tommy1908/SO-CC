Ejercicio 5
La siguiente función se utiliza para verificar la contraseña del usuario actual user en el programa que desbloquea la sesión luego de activarse el protector de pantalla del sistema.
El usuario ingresa la misma por teclado.

```c
1 bool check_pass(const char* user) {
2    char pass[128], realpass[128], salt[2];
3    // Carga la contraseña (encriptada) desde /etc/shadow
4    load_pass_from(realpass, sizeof(realpass), salt, user, "/etc/shadow");
5
6    // Pregunta al usuario por la contraseña.
7    printf("Password: ");
8    gets(pass);
9
10   // Demora de un segundo para evitar abuso de fuerza bruta
11   sleep(1);
12
13   // Encripta la contraseña y compara con la almacenada
14   return strcmp(crypt(pass, salt), realpass) == 0;
15 }
```

`void load_pass_from(buf, buf_size, salt, user, file)` lee del archivo file la contraseña encriptada del usuario user y la almacena en el buffer buf escribiendo a lo sumo buf_size-1 caracteres y un "\0" al final de estos; guarda además en salt el valor de los dos caracteres que se usaron para encriptar la contraseña guardada en file. Si el usuario no se encuentra, se
almacena en buf un valor de contraseña inválido, no generable por la función crypt.

`char* crypt(pass, salt)` devuelve la contraseña pass encriptada usando el valor salt.

`/etc/shadow` almacena información sensible del sistema y un usuario común no tiene acceso a este archivo, tiene permisos
`r-- --- --- root root /etc/shadow.`

a) La línea 4 del código hace un llamado a función que debe leer el archivo /etc/shadow, protegido
para los usuarios sin privilegio. Explicar qué mecanismo permite lanzar este programa a un usuario normal y hace que éste tenga acceso a /etc/shadow sin alterar los permisos del archivo.

b) Explicar por qué esta función tiene problemas de seguridad. ¿Qué datos sensibles del programa
controla el usuario?

c) Si solo los usuarios que ya ingresaron al sistema, que de por sí pueden ejecutar cualquier programa
desde la terminal, son los que pueden utilizar el protector de pantalla y por tanto esta función,
¿este problema de seguridad en el protector de pantalla del sistema compromete la integridad del
sistema completo? Justificar.
