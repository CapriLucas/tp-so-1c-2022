#include "memoria.h"

t_log* log_Memoria;
t_config_Memoria* config_Memoria;
int memoriaFd;
int kernelFd;
int cpuFd;


int main(){

    inicializar_proceso();

    // Test load memoria.config -- Borrar
    printf("PUERTO_ESCUCHA: %u\n", config_Memoria->PUERTO_ESCUCHA);
    printf("TAM_MEMORIA: %u\n", config_Memoria->TAM_MEMORIA);
    printf("TAM_PAGINA: %u\n", config_Memoria->TAM_PAGINA);
    printf("ENTRADAS_POR_TABLA: %u\n", config_Memoria->ENTRADAS_POR_TABLA);
    printf("RETARDO_MEMORIA: %u\n", config_Memoria->RETARDO_MEMORIA);
    printf("ALGORITMO_REEMPLAZO: %s\n", config_Memoria->ALGORITMO_REEMPLAZO);
    printf("MARCOS_POR_PROCESO: %u\n", config_Memoria->MARCOS_POR_PROCESO);
    printf("RETARDO_SWAP: %u\n", config_Memoria->RETARDO_SWAP);
    printf("PATH_SWAP: %s\n", config_Memoria->PATH_SWAP);

    // Iniciar server
    memoriaFd = iniciar_servidor (
        log_Memoria,
        "MEMORIA",
        "127.0.0.1",
        "8002"      // config_Memoria->PUERTO_ESCUCHA
    );

    if (!memoriaFd) {
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de CPU
    cpuFd = esperar_cliente (
        log_Memoria, 
        "CPU", 
        memoriaFd
    );
    
    pthread_t THREAD_KERNEL_LISTENER;
    if(!pthread_create(&THREAD_KERNEL_LISTENER, NULL, (void*) handler_kernel, NULL))
        pthread_detach(THREAD_KERNEL_LISTENER);
    else {
        log_error(log_Memoria, "ERROR CRITICO INICIANDO EL LISTENER DEL KERNEL. ABORTANDO.");
        return EXIT_FAILURE;
    }
    
    while(1);
    cerrar_programa();

}