```c
struct PCB {
int STAT; // valores posibles KE_RUNNING, KE_READY, KE_BLOCKED, KE_NEW
int P_ID; // process ID
int PC; // valor del PC del proceso al ser desalojado
int RO; // valor del registro R0 al ser desalojado
...
int R15; // valor del registro R15 al ser desalojado
int CPU_TIME // tiempo de ejecución del proceso
}

void Ke_context_switch(PCB* pcb_0, PCB* pcb_1){
    //Guardar el actual
    pcb_0->PC := PC ;
    pcb_0->R0 := R0;
    ... 
    pcb_0->R15 := R15;
    pcb_0->CPU_TIME := pcb_0->CPU_TIME + ke_current_user_time();;

    //Cargamos todos menos el PC (uso general) del nuevo proceso
    R0 := pcb_1->R0;
    ...
    R15 := pcb_1->R15;

    //Cambiar proceso actual
    pcb_0->STAT := KE_READY; 
    set_current_process(pcb_1->P_ID);
    pcb_1->STAT := KE_RUNNING;

    ke_reset_current_user_time();
    ret(); //cargar el proceso actual y volver modo usuario con el pc que tenia ese proceso
}
```