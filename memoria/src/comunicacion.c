#include "comunicacion.h"

void handler_kernel(){
    // Esperar conexión de Kernel
    kernelFd = esperar_cliente (
        log_Memoria, 
        "MEMORIA", 
        memoriaFd
    );

    while(1){
        t_paquete* paquete = malloc(sizeof(t_paquete));

        if(recibir_header(paquete, kernelFd) == 0){
            return;
        }
        
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