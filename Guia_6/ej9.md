Ejercicio 9
Linux permite pasar un descriptor de archivo de un proceso a otro (es decir, el valor de retorno de la syscall open()). 
Suponga que un proceso abre el directorio /home que está sobre una partición de ext2 y le envía el descriptor del archivo home a otro proceso con los permisos necesarios para poder leerlo.
Suponiendo que tiene la función auxiliar Ext2FSInode load_inode(int inode_number) que dado un número de inodo devuelve la estructura del inodo, escriba el pseudocódigo que le permita obtener el nombre del directorio (home) a partir del descriptor de archivo recibido.

```c
obtener_nombre_de_directorio_actual(fd){
    fs_dir_entry entry;
    bool found_me, found_parent;
    int me, parent;
    index=0;
    while(true){ // Termina porque tengo garantizado que existe, como no es un inodo no podemos saber el size
        read(entry,fd, max_possible_size); // No sabemos el tamaño por el nombre, pero en este caso podriamos hacer match por tamaño quizas, es "." lo que buscamos. De todas formas..
        if(entry.size == 1 && strcmp(entry.name == '.') == 0){
            me = entry.inode
            found_me=1
        }
        if(entry.size == 2 && strcmp(entry.name == '..') == 0){
            parent = entry.inode
            found_me=1
        }
        if(found_me&&found_parent){
            break;
        }

        index += record_length;
    }

    Ext2FSInode parent load_inode(parent); // ahora tenemos que buscar la entry cuyo inodo sea el numero del neustro
    while(true){ // Termina porque tengo garantizado que yo existo
        read(entry,fd, max_possible_size);
        if(entry.inode == me){
            return entry.name
        }
    }
}

```