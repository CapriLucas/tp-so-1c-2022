#include "cpu.h"

t_log* mainLog;
t_config_CPU* mainConfig;


static void inicializar_proceso() {
    mainLog = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_CPU));
    if (!cargar_configuracion(mainConfig, mainLog)) {
        exit(EXIT_FAILURE);
    }
}


int main(){

    inicializar_proceso();

    // Test config.
    printf("ENTRADAS_TLB: %d\n", mainConfig->ENTRADAS_TLB);
    printf("REEMPLAZO_TLB: %s\n", mainConfig->REEMPLAZO_TLB);
    printf("RETARDO_NOOP: %d\n", mainConfig->RETARDO_NOOP);
    printf("IP_MEMORIA: %s\n", mainConfig->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %d\n", mainConfig->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA_DISPATCH: %d\n", mainConfig->PUERTO_ESCUCHA_DISPATCH);
    printf("PUERTO_ESCUCHA_INTERRUPT: %d\n", mainConfig->PUERTO_ESCUCHA_INTERRUPT);

    int kernelFd;
/* 
    if(!generar_conexiones(&kernelFd, mainConfig, mainLog)){
        cerrar_programa(mainConfig, mainLog, &kernelFd);
        return EXIT_FAILURE;
    }
*/

    cerrar_programa(mainConfig, mainLog, &kernelFd);

}
