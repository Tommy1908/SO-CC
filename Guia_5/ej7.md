Ejercicio 7 ⋆
Se desea implementar el driver de una controladora de una vieja unidad de discos ópticos que requiere controlar manualmente el motor de la misma.
Esta controladora posee 3 registros de lectura y 3 de escritura. 

Los registros de escritura son:
- DOR_IO: enciende (escribiendo 1) o apaga (escribiendo 0) el motor de la unidad.
- ARM: número de pista a seleccionar.
- SEEK_SECTOR: número de sector a seleccionar dentro de la pista.

Los registros de lectura son:

- DOR_STATUS: contiene el valor 0 si el motor está apagado (o en proceso de apagarse), 1 si está encendido. Un valor 1 en este registro no garantiza que la velocidad rotacional del motor sea la suficiente como para realizar exitosamente una operación en el disco.
- ARM_STATUS: contiene el valor 0 si el brazo se está moviendo, 1 si se ubica en la pista indicada en el registro ARM.
- DATA_READY: contiene el valor 1 cuando el dato ya fue enviado.

Además, se cuenta con las siguientes funciones auxiliares (ya implementadas):

- int cantidad_sectores_por_pista(): Devuelve la cantidad de sectores por cada pista del disco. El sector 0 es el primer sector de la pista.
- void escribir_datos(void *src): Escribe los datos apuntados por src en el último sector seleccionado.
- void sleep(int ms): Espera durante ms milisegundos.

Antes de escribir un sector, el driver debe asegurarse que el motor se encuentre encendido.
Si no lo está, debe encenderlo, y para garantizar que la velocidad rotacional sea suficiente, debe esperar al menos 50 ms antes de realizar cualquier operación. 
A su vez, para conservar energía, una vez que finalice una operación en el disco, el motor debe ser apagado. 
El proceso de apagado demora como máximo 200 ms, tiempo antes del cual no es posible comenzar nuevas operaciones.

a) Implementar la función write(int sector, void *data) del driver, que escriba los datos apuntados por data en el sector en formato LBA indicado por sector. Para esta primera implementación, no usar interrupciones.

```c
semaphore mutex;

driver_write(int sector, void *data){
    sema_wait(&mutex);
    if(IN(DOR_STATUS)==0){ //Si esta apagado prendo
        OUT(DOR_IO,1);
        sleep(50);
    }

    int sectores_por_pista = cantidad_sectores_por_pista();
    int pista = sector / sectores_por_pista;
    int offest = sector % sectores_por_pista;

    OUT(ARM,pista);
    OUT(SEEK_SECTOR,offset);

    while(IN(ARM_STATUS)==0){} //Busy waiting brazo en movimiento

    escribir_datos(data);  
    
    while(IN(DATA_READY)==0){} // Esperar que se envie el dato

    OUT(DOR_IO,0); // Apago
    sleep(200);

    sema_signal(&mutex);
}

int read(void *data) {return IO_ERROR;}
int driver_init(){
    sema_init(&mutex, 1);
    return IO_OK; 
}
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove() { return IO_OK; }
```

b) Modificar la función del inciso anterior utilizando interrupciones. La controladora del disco realiza una interrupción en el IRQ 6 cada vez que los registros ARM_STATUS o DATA_READY toman el valor 1. 
Además, el sistema ofrece un timer que realiza una interrupción en el IRQ 7 una vez cada 50ms.
Para este inciso, no se puede utilizar la función sleep.

```c
semaphore mutex;
semaphore sem_brazo;
semaphore sem_dato;
semaphore timer;

bool timer_on = 0;
bool waiting_for_movement = 0;
bool waiting_for_data = 0;

driver_write(int sector, void *data){
    sema_wait(&mutex);
    if(IN(DOR_STATUS)==0){ //Si esta apagado prendo
        timer_on = 1;
        OUT(DOR_IO,1);
        //sleep(50);
        sema_wait(%timer);
        sema_wait(%timer);// Una mas por si la interrupcion salto cuanyo yo llevaba esperando 25ms porque tarde 25 ms en llegar aca
        timer_on = 0;
    }

    int sectores_por_pista = cantidad_sectores_por_pista();
    int pista = sector / sectores_por_pista;
    int offest = sector % sectores_por_pista;

    waiting_for_movement = 1;

    OUT(ARM,pista);
    OUT(SEEK_SECTOR,offset);

    sema_wait(%sem_brazo);
    waiting_for_movement = 0;
    //while(IN(ARM_STATUS)==0){} //Busy waiting brazo en movimiento

    waiting_for_data = 1;
    escribir_datos(data);  
    sema_wait(%sem_dato);
    waiting_for_data = 0;
    //while(IN(DATA_READY)==0){} // Esperar que se envie el dato

    timer_on = 1;
    OUT(DOR_IO,0); // Apago
    //sleep(200);
    sema_wait(%timer);
    sema_wait(%timer);
    sema_wait(%timer);
    sema_wait(%timer);
    sema_wait(%timer); // Mismo argumento que arriba
    timer_on = 0;


    sema_signal(&mutex);

}

void handler_status(){
    if(waiting_for_movement){ // Para no acumular. Ademas, asumimos que no pueden llegar 2 a la vez, no tiene sentido, ni llegarme una que no es la que estoy esperando, solo yo las llamo
        if(IN(ARM_STATUS)==1)
        {
            sema_signal(&sem_brazo);
        }
    }
    if(waiting_for_data){   
        if(IN(DATA_READY)==1)
        {
            sema_signal(&sem_dato);
        }
    }
}
void handler_timer(){
    if(timer_on){
        sema_signal(%timer);
    }
}

int read(void *data) {return IO_ERROR;}
int driver_init(){
    sema_init(&mutex, 1);
    sema_init(&sem_brazo, 0);
    sema_init(&sem_dato, 0);
    sema_init(&timer, 0);
    request_irq(6, handler_status);
    request_irq(7, handler_timer);
    return IO_OK; 
}
int driver_open()   { return IO_OK; }
int driver_close()  { return IO_OK; }
int driver_remove() { 
    free_irq(6);
    free_irq(7);
    sema_destroy(&mutex);
    sema_destroy(&sem_brazo);
    sema_destroy(&sem_dato);
    sema_destroy(&timer);
    return IO_OK; 
    }
```