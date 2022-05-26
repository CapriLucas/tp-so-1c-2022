#include "kernel.h"

t_log* mainLog;
t_config_kernel* mainConfig;
int server_fd;

static void initializeProcess(){
    mainLog = log_create("./logs/kernel.log", "KERNEL", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_kernel));
    if (!cargar_configuracion(mainConfig, mainLog) || !crear_servidor(mainConfig, mainLog, &server_fd, SERVERNAME)){
        liberar_conexion(&server_fd);
        exit(EXIT_FAILURE);
    }
}

int main(){
    initializeProcess();
    
    int memoriaFd;
    int kernelDispatchFd;
    int kernelInterruptFd;

    // Test load kernel.config --Borrar
    printf("IP_MEMORIA: %s\n", mainConfig->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %u\n", mainConfig->PUERTO_MEMORIA);
    printf("IP_CPU: %s\n", mainConfig->IP_CPU);
    printf("PUERTO_CPU_DISPATCH: %u\n", mainConfig->PUERTO_CPU_DISPATCH);
    printf("PUERTO_CPU_INTERRUPT: %u\n", mainConfig->PUERTO_CPU_INTERRUPT);
    printf("PUERTO_ESCUCHA: %u\n", mainConfig->PUERTO_ESCUCHA);
    printf("ALGORITMO_PLANIFICACION: %s\n", mainConfig->ALGORITMO_PLANIFICACION);
    printf("ESTIMACION_INICIAL: %u\n", mainConfig->ESTIMACION_INICIAL);
    printf("ALFA: %f\n", mainConfig->ALFA);
    printf("GRADO_MULTIPROGRAMACION: %u\n", mainConfig->GRADO_MULTIPROGRAMACION);
    printf("TIEMPO_MAXIMO_BLOQUEADO: %u\n", mainConfig->TIEMPO_MAXIMO_BLOQUEADO);


    // Crear conexión con MEMORIA
    memoriaFd = crear_conexion (
        mainLog, 
        "MEMORIA", 
        config->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

    if (!memoriaFd) {    
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    // Crear conexión con CPU para mensajes de dispatch  
    kernelDispatchFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8001"  // config->PUERTO_CPU_DISPATCH 
    );
     
    // Crear conexión con CPU para mensajes de interrupciones
    kernelInterruptFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8005"  // config->PUERTO_ESCUCHA_INTERRUPT
    );
    

    while(server_escuchar(SERVERNAME, server_fd, mainLog));
    cerrar_programa(mainConfig, mainLog, &server_fd);
}