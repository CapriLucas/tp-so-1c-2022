#include "kernel.h"

t_log* mainLog;
t_config_kernel* mainConfig;
int server_fd;
uint32_t current_pid;

int main(){
    initializeKernel();
    
    int memoriaFd;
    int kernelDispatchFd;
    int kernelInterruptFd;

    pthread_t THREAD_LARGO_PLAZO;
    if(!pthread_create(&THREAD_LARGO_PLAZO, NULL, (void*) handler_largo_plazo, NULL))
        pthread_detach(THREAD_LARGO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE LARGO PLAZO. ABORTANDO.");
        return EXIT_FAILURE;
    }

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

    // Crear conexión con CPU para mensajes de dispatch  
    kernelDispatchFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8001"  // config->PUERTO_CPU_DISPATCH 
    );

    // Test send dispatch msg to CPU -- Borrar
    send(kernelDispatchFd, "DISPATCH MSG", 13, 0);
     
    // Crear conexión con CPU para mensajes de interrupciones
    kernelInterruptFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8005"  // config->PUERTO_ESCUCHA_INTERRUPT
    );

    // Test send interrupt msg to CPU -- Borrar
    send(kernelInterruptFd, "INTERRUPT MSG", 14, 0);
    
/*     
    char* msg_interrupt = "Mensaje interrupt";
    send(kernelDispatchFd, msg_dispatch, sizeof(msg_dispatch, 0));
 */

    //TODO ver como sacar esta espera activa del main thread
    while(1);
    cerrar_programa(mainConfig, mainLog, &server_fd);
}