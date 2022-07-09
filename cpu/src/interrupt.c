#include "interrupt.h"


void interrupt_server() {

    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    char* puerto_interrupt = string_itoa(config_CPU->PUERTO_ESCUCHA_INTERRUPT);
    cpuInterruptFd = iniciar_servidor (
        log_CPU,
        "CPU",
        "127.0.0.1", 
        puerto_interrupt
    );

    if (!cpuInterruptFd) {    
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de KERNEL para mensajes de interrupciones
    kernelInterruptFd = esperar_cliente (
        log_CPU, 
        "KERNEL INTERRUPT", 
        cpuInterruptFd
    );

    if (!kernelInterruptFd) {    
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    while (1) {
        t_paquete* paquete = malloc(sizeof(t_paquete));

        if(recibir_header(paquete, kernelInterruptFd) == 0){
            return;
        }
        if(paquete->codigo_operacion != MSG_INTERRUPT){
            log_info(log_CPU, "Se espera recibir MSG_INTERRUPT codigo de operacion");
            return;
        }

        pthread_mutex_lock(&MUTEX_INTERRUPT);
        interrupt = true;
        pthread_mutex_unlock(&MUTEX_INTERRUPT);

        eliminar_paquete(paquete);
    }

}
