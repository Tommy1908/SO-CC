Se desea escribir un driver para la famosa impresora Headaches Persistent. El manual del controlador nos dice que para comenzar una impresión, se debe:

- Ingresar en el registro de 32 bits LOC_TEXT_POINTER la dirección de memoria dónde empieza el buffer que contiene el string a imprimir.
- Ingresar en el registro de 32 bits LOC_TEXT_SIZE la cantidad de caracteres que se deben leer del buffer.
- Colocar la constante START en el registro LOC_CTRL.

En este momento, si la impresora detecta que no hay suficiente tinta para comenzar, escribirá rápidamente el valor LOW_INK en el registro LOC_CTRL y el valor READY en el registro LOC_STATUS.

Caso contrario, la impresora comenzará la impresión, escribiendo el valor PRINTING en el registro LOC_CTRL y el valor BUSY en el registro LOC_STATUS. Al terminar, la impresora escribirá el valor FINISHED en el registro LOC_CTRL y el valor READY en el registro LOC_STATUS. Un problema a tener en cuenta es que, por la mala calidad del hardware, éstas impresoras suelen detectar erróneamente bajo nivel de tinta. Sin embargo, el fabricante nos asegura en el manual que “alcanza con probar hasta 5 veces para saber con certeza si hay o no nivel bajo de tinta”.

El controlador soporta además el uso de las interrupciones: HP_LOW_INK_INT, que se lanza cuando la impresora detecta que hay nivel bajo de tinta, y HP_FINISHED_INT, que se lanza al terminar una impresión.

Se pide implementar las funciones int driver_init(), int driver_remove() y int driver_write() del driver. Piense cuidadosamente si conviene utilizar polling, interrupciones o una mezcla de ambos. Justifique la elección. Además, debe asegurese de que el código no cause condiciones de carrera. Las impresiones deberán ser bloqueantes. No hace falta que implemente spooling.

Vamos a usar pooling para la tinta, ya que nos aclara que es MUY rapido, y asi evitamos el overhead de tener una interrupcion y la complejidad de programarlo.
Usamos interrupciones para HP_FINISHED_INT, asi evitamos el pooling.

```c
semaphore mutex;
semaphore terminado;

driver_write(void *data){
    sema_wait(&mutex);

    OUT(LOC_TEST_POINTER, data->string); // tendria que haber copiado primero los datos a un buffer con el copy from user y usar un kmalloc para los datos
    OUT(LOC_TEXT_SIZE, data->size);
    OUT(LOC_CTRL, START);

    try=0;
    while((estado = IN(LOC_CTRL)) != PRINTING){
        // Si 5 intentos da low ink error, si logro empezar a imprimir en el medio OK
        if(estado == LOW_INK && try < 5){
            try++;
        }
        else{
            sema_signal(&mutex);
            return IO_ERROR;
        }
    }

    sema_wait(&terminado);
    sema_signal(&mutex);
    return IO_OK;

}

void handler(){
    sema_signal(&terminado);
}

int read(void *data) {return IO_ERROR;}
int driver_init()   { 
    sem_init(&mutex,1);
    sem_init(&terminado,0);
    request_irq(HP_FINISHED_INT, handler);
    return IO_OK;
    }
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove() { 
    destroy(&mutex);
    destroy(&terminado);
    free_irq(HP_FINISHED_INT);
    return IO_OK; 
    }
```