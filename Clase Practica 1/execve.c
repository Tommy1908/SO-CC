# include <unistd.h>
# include <stdio.h>

int main(){
    // 1. Definimos los argumentos
    char *args[] = {"/bin/ls", "-l", NULL}; 
    
    // 2. Definimos el entorno (vacío en este caso)
    char *env[] = {NULL};
    
    // 3. Llamamos a execve
    execve("/bin/ls", args, env);

    return 0;
}
