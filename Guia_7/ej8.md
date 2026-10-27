Ejercicio 8 ⋆
Suponiendo que el usuario controla el parámetro dir, el siguiente código, que corre con mayor nivel de privilegio, intenta mostrar en pantalla el directorio dir usando el comando ls. Sin embargo, tiene problemas de seguridad.

```c
#define BUF_SIZE 1024
void wrapper_ls(const char * dir) {
    char cmd[BUF_SIZE];
    snprintf(cmd, BUF_SIZE-1, "ls %s", dir);
    system(cmd);
}
```

a) Muestre un valor de la cadena dir que además de listar el directorio actual muestre el contenido del archivo /etc/passwd.
Okey, podria enviar ". & cat /etc/passwd" o ". ; cat /etc/passwd"
La idea, es que tengo acceso a la terminal, y puedo concatenar acciones, incluso haciendo cosas a las que el usuario no tiene permiso, escalando de privilegios. Si me entra en 1024 puedo hacer lo que sea.
b) Posteriormente se reemplazó esta por la función secure_wrapper_ls donde el tercer parámetro de snprintf en vez de ser "ls %s" se reemplaza por "ls \" %s\"". Muestre que la modificación no soluciona el problema de seguridad.
Okey, esto hace que me agrega las comillas, como que leeria literalmente  ;cat /etc/paswd, como si fuera el parametro para ls. Pero como es texto, podemos agregar nuestras propias comillas y romperle todo otra vez
envio -> `." ; cat /etc/passwd #`
Me quedaria `ls "." ; cat /etc/passwd #"`. El # comenta la comilla final sin dar error
Sino podriamos mandar `." ; cat /etc/passwd ; ls".` y seria 2 ls, y evitamos el comentario.
c) Posteriormente se agregó una verificación de la cadena dir: no puede contener ningún carácter “;”. Muestre que esta modificación tampoco soluciona el problema.
Envio `." & cat /etc/passwd #` o `." && cat /etc/passwd #`
d) Proponga una versión de esta función que no tenga problemas de seguridad
Que no permita usar ningun simbolo que permita seguir ejecutando cosas supongo. Nose cuales son todos. Otra solucion seria si de alguna forma se puede limitar a 1 sola cosas correr. o algo asi qcyo, quizas el exec con el parametro ls, nose si te tira error al intentar correr otra cosa. Tipo terminal que SOLO ejecuta ls.Podrias limitar el tamaño pero creo que es menoss util y mas problematico

Se debe evitar el uso de system(), ya que interpreta la entrada del usuario mediante una shell, permitiendo ejecutar comandos adicionales.

Una solución es utilizar fork() y exec, pasando el directorio como un argumento independiente al ejecutable ls. De esta manera, los caracteres especiales ingresados por el usuario no son interpretados como operadores de la shell, sino como parte del nombre del directorio.
Ahi va, era por ahi. Podes hacer lo de evitar los caracteres pero eventualmente podria romperse. 

```c
void wrapper_ls(const char *dir) {
    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/ls", "ls", "--", dir, (char *)NULL);
        _exit(127);
    }

    if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
}
```
ls intenta listar literalmente un directorio con ese nombre. No ejecuta cat.
El argumento "--" también es importante: evita que un directorio cuyo nombre comienza con - sea interpretado como una opción de ls.
Esto elimina la inyección de comandos por la shell, aunque en un programa real con privilegios elevados todavía habría que considerar otras cuestiones de seguridad, como qué directorios puede consultar el usuario y cómo gestionar errores.