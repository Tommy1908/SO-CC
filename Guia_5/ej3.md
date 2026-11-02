Ejercicio 3
Una tecla posee un único registro de E/S : BTN_STATUS. Solo el bit menos significativo y el segundo bit menos significativo son de interés:

- BTN_STATUS0: vale 0 si la tecla no fue pulsada, 1 si fue pulsada.
- BTN_STATUS1: escribir 0 en este bit para limpiar la memoria de la tecla.

Escribir un driver para manejar este dispositivo de E/S. 
El driver debe retornar la constante BTN_PRESSED cuando se presiona la tecla. Usar busy waiting.

```c
int driver_read(void *data) {
    int estado;

    do {
        estado = IN(BTN_STATUS);
    } while ((estado & 0b1) == 0);


    OUT(BTN_STATUS, estado & ~(1 << 1)); //pone en 0 el bit 1

    int resultado = BTN_PRESSED;

    if (copy_to_user((char *) data,
                     (char *) &resultado,
                     sizeof(resultado)) != 0) {
        return IO_ERROR;
    }

    return IO_OK;
}

int driver_write(void *data) {return IO_ERROR;}
int driver_init()   { return IO_OK; }
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove(){ return IO_OK; }
```