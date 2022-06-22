#include "memoria.h"

t_log* mainLog;
t_config_Memoria* mainConfig;
int memoriaFd;
int kernelFd;
int cpuFd;

static void inicializar_proceso() {
    mainLog = log_create("./logs/memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_Memoria));
    if (!cargar_configuracion(mainConfig, mainLog)) {
        exit(EXIT_FAILURE);
    }
}


int main(){

    inicializar_proceso();


    // Test load memoria.config -- Borrar
    // printf("PUERTO_ESCUCHA: %u\n", mainConfig->PUERTO_ESCUCHA);
    // printf("TAM_MEMORIA: %u\n", mainConfig->TAM_MEMORIA);
    // printf("TAM_PAGINA: %u\n", mainConfig->TAM_PAGINA);
    // printf("ENTRADAS_POR_TABLA: %u\n", mainConfig->ENTRADAS_POR_TABLA);
    // printf("RETARDO_MEMORIA: %u\n", mainConfig->RETARDO_MEMORIA);
    // printf("ALGORITMO_REEMPLAZO: %s\n", mainConfig->ALGORITMO_REEMPLAZO);
    // printf("MARCOS_POR_PROCESO: %u\n", mainConfig->MARCOS_POR_PROCESO);
    // printf("RETARDO_SWAP: %u\n", mainConfig->RETARDO_SWAP);
    // printf("PATH_SWAP: %s\n", mainConfig->PATH_SWAP);

    // Iniciar server
    memoriaFd = iniciar_servidor (
        mainLog,
        "MEMORIA",
        "127.0.0.1",
        "8002"      // mainConfig->PUERTO_ESCUCHA
    );

    // Esperar conexión de CPU
    cpuFd = esperar_cliente (
        mainLog, 
        "CPU", 
        memoriaFd
    );
    
    pthread_t THREAD_KERNEL_LISTENER;
    if(!pthread_create(&THREAD_KERNEL_LISTENER, NULL, (void*) handler_kernel, NULL))
        pthread_detach(THREAD_KERNEL_LISTENER);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL LISTENER DEL KERNEL. ABORTANDO.");
        return EXIT_FAILURE;
    }
    
    while(1);
    cerrar_programa(mainConfig, mainLog, &memoriaFd);

}