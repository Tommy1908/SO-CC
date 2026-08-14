#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t senal_recibida = 0;

void handler(int sig) {
    senal_recibida = 1;
}

int main(void) {
    struct sigaction sa = { .sa_handler = handler };
    sigaction(SIGINT, &sa, NULL);

    sigset_t mascara_bloqueada, mascara_original;
    sigemptyset(&mascara_bloqueada);
    sigaddset(&mascara_bloqueada, SIGINT);

    // 1. BLOQUEAR SIGINT
    // A partir de este momento, si llega Ctrl+C, el kernel la guarda como PENDIENTE
    // pero no interrumpe el programa.
    sigprocmask(SIG_BLOCK, &mascara_bloqueada, &mascara_original);

    /* --- SECCIÓN CRÍTICA / PREPARACIÓN --- */
    // Aunque metamos un sleep() de 5 segundos aquí y el usuario presione Ctrl+C,
    // la señal NO se pierde: queda retenida por el kernel.
    sleep(5); 

    // 2. ESPERA ATÓMICA CON sigsuspend()
    // Pasa la máscara original (donde SIGINT NO está bloqueada).
    // El kernel, de forma ATÓMICA:
    //   a) Desbloquea SIGINT.
    //   b) Pone a dormir el proceso.
    while (!senal_recibida) {
        // Se ejecuta el handler, sigsuspend() retorna -1 (errno = EINTR) 
        // y restaura automáticamente la máscara que bloqueaba SIGINT.
        sigsuspend(&mascara_original); 
    }

    // 3. RESTAURAR MÁSCARA FINAL
    sigprocmask(SIG_SETMASK, &mascara_original, NULL);

    printf("Señal recibida de forma segura sin condiciones de carrera.\n");
    return EXIT_SUCCESS;
}