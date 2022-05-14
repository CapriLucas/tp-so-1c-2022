#include "memoria.h"

t_log* mainLog;
t_config_Memoria* mainConfig;

static void inicializar_proceso() {
    mainLog = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_Memoria));
    if (!cargar_configuracion(mainConfig, mainLog)) {
        exit(EXIT_FAILURE);
    }
}


int main(){

    inicializar_proceso();


    // Test config --- Borrar.
    printf("PUERTO_ESCUCHA: %d\n", mainConfig->PUERTO_ESCUCHA);
    printf("TAM_MEMORIA: %d\n", mainConfig->TAM_MEMORIA);
    printf("TAM_PAGINA: %d\n", mainConfig->TAM_PAGINA);
    printf("ENTRADAS_POR_TABLA: %d\n", mainConfig->ENTRADAS_POR_TABLA);
    printf("RETARDO_MEMORIA: %d\n", mainConfig->RETARDO_MEMORIA);
    printf("ALGORITMO_REEMPLAZO: %s\n", mainConfig->ALGORITMO_REEMPLAZO);
    printf("MARCOS_POR_PROCESO: %d\n", mainConfig->MARCOS_POR_PROCESO);
    printf("RETARDO_SWAP: %d\n", mainConfig->RETARDO_SWAP);
    printf("PATH_SWAP: %s\n", mainConfig->PATH_SWAP);

  
    int kernelFd;
/* 
    if(!generar_conexiones(&kernelFd, mainConfig, mainLog)){
        cerrar_programa(mainConfig, mainLog, &kernelFd);
        return EXIT_FAILURE;
    }
*/

    cerrar_programa(mainConfig, mainLog, &kernelFd);

}
