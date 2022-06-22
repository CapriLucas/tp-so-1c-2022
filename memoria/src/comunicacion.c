#include "comunicacion.h"

uint8_t recibir_header(t_paquete* paquete);

void handler_kernel(){
    // Esperar conexión de Kernel
    kernelFd = esperar_cliente (
        mainLog, 
        "MEMORIA", 
        memoriaFd
    );

    while(1){
        t_paquete* paquete = malloc(sizeof(t_paquete));
        recibir_header(paquete);
        switch(paquete->codigo_operacion){
            case CREAR_PROCESO_EN_MEMORIA:
                crear_proceso(paquete);
                break;
            //TODO implmentar delete y suspend
            default:
                return;
        }
        free(paquete);
    }
}

uint8_t recibir_header(t_paquete* paquete){
    paquete->buffer = malloc(sizeof(t_buffer));

    if(recv(kernelFd, &(paquete->codigo_operacion), sizeof(op_code), 0) == 0) {
        log_info(mainLog, "DISCONNECTED!!!");
        return 0;
    }

    recv(kernelFd, &(paquete->buffer->size), sizeof(uint32_t), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(kernelFd, paquete->buffer->stream, paquete->buffer->size, 0);

    return 1;
}