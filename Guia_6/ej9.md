Ejercicio 9
Linux permite pasar un descriptor de archivo de un proceso a otro (es decir, el valor de retorno de la syscall open()). 
Suponga que un proceso abre el directorio /home que está sobre una partición de ext2 y le envía el descriptor del archivo home a otro proceso con los permisos necesarios para poder leerlo.
Suponiendo que tiene la función auxiliar Ext2FSInode load_inode(int inode_number) que dado un número de inodo devuelve la estructura del inodo, escriba el pseudocódigo que le permita obtener el nombre del directorio (home) a partir del descriptor de archivo recibido.

```c
char* obtener_nombre(int fd) {
    int me =-1, parent = -1;
    char buffer[BLOCK_SIZE];
    int bytes;

    // Buscar "." y ".." en el directorio
    while ((bytes = read(fd, buffer, BLOCK_SIZE)) > 0) {
        int offset = 0;

        while (offset < bytes) {
            Ext2FSDirEntry* entry = (Ext2FSDirEntry*)(buffer + offset);

            if (entry->name_length == 1 && strncmp(entry->name, ".", 1) == 0) {
                me = entry->inode;
            }

            if (entry->name_length == 2 && strncmp(entry->name, "..", 2) == 0) {
                parent = entry->inode;
            }

            offset += entry->record_length; // supongamos que siempre leemos todo bien
        }

        if (me != -1 && parent != -1) // Es condicion de corte, siempre existen
            break;
    }

    Ext2FSInode parent_inode = load_inode(parent);

    int cantidad = parent_inode.size / BLOCK_SIZE;

    for (int i = 0; i < cantidad; i++) {
        int block_addr = get_block_address(&parent_inode, i);

        read_block(block_addr, buffer);

        int offset = 0;

        while (offset < BLOCK_SIZE) {
            Ext2FSDirEntry* entry = (Ext2FSDirEntry*)(buffer + offset);

            if (entry->inode == me) {
                return copiar_nombre(entry->name, entry->name_length);
            }

            offset += entry->record_length;
        }
    }

    return NULL;
}
```