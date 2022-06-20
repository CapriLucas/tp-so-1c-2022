#include "kernel.h"

t_log* mainLog;
t_config_kernel* mainConfig;
int server_fd;
int memoriaFd;
int kernelDispatchFd;
int kernelInterruptFd;
uint32_t next_pid;

int main(){
    initializeKernel();

    // Crear conexión con MEMORIA
    memoriaFd = crear_conexion (
        mainLog, 
        "MEMORIA", 
        mainConfig->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

    if (!memoriaFd) {    
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    // Crear conexión con CPU (dispatch)  
    kernelDispatchFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8001"  // mainConfig->PUERTO_CPU_DISPATCH 
    );

    // Test send dispatch msg to CPU -- Borrar
    send(kernelDispatchFd, "DISPATCH MSG", 13, 0);
     
    // Crear conexión con CPU (interrupciones)
    kernelInterruptFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8005"  // mainConfig->PUERTO_CPU_INTERRUPT
    );

    // Test send interrupt msg to CPU -- Borrar
    send(kernelInterruptFd, "INTERRUPT MSG", 14, 0);


    // Comienza planificado de largo plazo
    pthread_t THREAD_LARGO_PLAZO;
    if(!pthread_create(&THREAD_LARGO_PLAZO, NULL, (void*) handler_largo_plazo, NULL))
        pthread_detach(THREAD_LARGO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE LARGO PLAZO. ABORTANDO.");
        return EXIT_FAILURE;
    }



    //TODO ver como sacar esta espera activa del main thread
    while(1);
    cerrar_programa(mainConfig, mainLog, &server_fd);
}