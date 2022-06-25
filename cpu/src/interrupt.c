#include "interrupt.h"


void interrupt_server() {

    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    cpuInterruptFd = iniciar_servidor (
        log_CPU,
        "CPU",
        "127.0.0.1", 
        "8005"  // config_CPU->PUERTO_ESCUCHA_INTERRUPT
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

}
