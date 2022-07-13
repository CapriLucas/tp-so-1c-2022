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
    char* puerto_memoria = string_itoa(mainConfig->PUERTO_MEMORIA);
    memoriaFd = crear_conexion (
        mainLog, 
        "MEMORIA", 
        mainConfig->IP_MEMORIA, 
        puerto_memoria
    );

    if (!memoriaFd) {    
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    // Comienza planificado de largo plazo
    pthread_t THREAD_LARGO_PLAZO;
    if(!pthread_create(&THREAD_LARGO_PLAZO, NULL, (void*) handler_largo_plazo, NULL))
        pthread_detach(THREAD_LARGO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE LARGO PLAZO. ABORTANDO.");
        return EXIT_FAILURE;
    }

    // Comienza planificador de corto plazo
    pthread_t THREAD_CORTO_PLAZO;
    if(!pthread_create(&THREAD_CORTO_PLAZO, NULL, (void*) handler_corto_plazo, NULL))
        pthread_detach(THREAD_CORTO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE CORTO PLAZO. ABORTANDO.");
        return EXIT_FAILURE;
    }

    // Comienza hilo i/o
    pthread_t THREAD_I_O;
    if(!pthread_create(&THREAD_I_O, NULL, (void*) handler_i_o, NULL))
        pthread_detach(THREAD_I_O);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL HILO I_O. ABORTANDO.");
        return EXIT_FAILURE;
    }

    // Comienza hilo mediano plazo
    pthread_t THREAD_MEDIANO_PLAZO;
    if(!pthread_create(&THREAD_MEDIANO_PLAZO, NULL, (void*) handler_mediano_plazo , NULL))
        pthread_detach(THREAD_MEDIANO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL planif mediano plazo. ABORTANDO.");
        return EXIT_FAILURE;
    }



    //TODO ver como sacar esta espera activa del main thread
    while(1);
    cerrar_programa(mainConfig, mainLog, &server_fd);
}