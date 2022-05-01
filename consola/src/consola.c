#include "consola.h"

t_log* mainLog;
t_config_consola* mainConfig;

static void initializeProcess(){
    mainLog = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_consola));
    if (!cargar_configuracion(mainConfig, mainLog)){
        exit(EXIT_FAILURE);
    }
}

int main(){
    initializeProcess();

    int kernelFd;
    if(!generar_conexiones(kernelFd, mainConfig, mainLog)){
        cerrar_programa(mainConfig, mainLog);
        return EXIT_FAILURE;
    }
    cerrar_programa(mainConfig, mainLog);
}