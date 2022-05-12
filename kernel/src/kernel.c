#include "kernel.h"

t_log* mainLog;
t_config_kernel* mainConfig;
int server_fd;

static void initializeProcess(){
    mainLog = log_create("./logs/kernel.log", "CONSOLA", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_kernel));
    if (!cargar_configuracion(mainConfig, mainLog) || !crear_servidor(mainConfig, mainLog, &server_fd, SERVERNAME)){
        liberar_conexion(&server_fd);
        exit(EXIT_FAILURE);
    }
}

int main(){
    initializeProcess();

    while(server_escuchar(SERVERNAME, server_fd, mainLog));
    cerrar_programa(mainConfig, mainLog, &server_fd);
}