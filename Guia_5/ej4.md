Ejercicio 4 ⋆
Reescribir el driver del ejercicio anterior para que utilice interrupciones en lugar de busy waiting. 
Para ello, aprovechar que la tecla ha sido conectada a la línea de interrupción número 7. Para indicar al dispositivo que debe efectuar una nueva interrupción al detectar una nueva pulsación de la tecla, debe guardar la constante BTN_INT en el registro de la tecla.
Ayuda: usar semáforos.

Una tecla posee un único registro de E/S : BTN_STATUS. Solo el bit menos significativo y el segundo bit menos significativo son de interés:

- BTN_STATUS0: vale 0 si la tecla no fue pulsada, 1 si fue pulsada.
- BTN_STATUS1: escribir 0 en este bit para limpiar la memoria de la tecla.

Escribir un driver para manejar este dispositivo de E/S. 
El driver debe retornar la constante BTN_PRESSED cuando se presiona la tecla. Usar busy waiting.

```c
semaphore sem;

int driver_read(void *data) {
    int estado;

    out(BTN_INT,);
    sema_wait(&sem);
    // Seccion crica
    estado = IN(BTN_STATUS);
    OUT(BTN_STATUS, estado & ~(1 << 1)); //pone en 0 el bit 1
    int resultado = BTN_PRESSED;


    if (copy_to_user((char *) data,
                     (char *) &resultado,
                     sizeof(resultado)) != 0) {
        return IO_ERROR;
    }

    return IO_OK;
}

void handler(){
    sema_signal(&sem);
}


int driver_write(void *data) {return IO_ERROR;}
int driver_init(){ 
    sema_init(&sem, 0);
    request_irq(7, handler);
    return IO_OK; 
    }
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove(){ return IO_OK; }
```