Un cronómetro posee 2 registros de E/S:

- CHRONO_CURRENT_TIME que permite leer el tiempo medido,
- CHRONO_CTRL que permite ordenar al dispositivo que reinicie el contador.

El cronómetro reinicia su contador escribiendo la constante CHRONO_RESET en el registro de control.
Escribir un driver para manejar este cronómetro.
Este driver debe devolver el tiempo actual cuando invoca la operación read(). Si el usuario invoca la operación write(), el cronómetro debe reiniciarse.

```c
int driver_read(int *data) {
    int tiempo = IN(CHRONO_CURRENT_TIME);

    if (copy_to_user((char *) data, (char *) &tiempo, sizeof(tiempo)) != 0) {
        return IO_ERROR;
    }

    return IO_OK;
}

int driver_write(int *data) {
    OUT(CHRONO_CTRL, CHRONO_RESET);
    return IO_OK;
}

int driver_init()   { return IO_OK; }
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove(){ return IO_OK; }
```
