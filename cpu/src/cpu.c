#include "cpu.h"

// Config & Log
t_config_CPU* config_CPU;
t_log* log_CPU;

// Sockets
int memoriaFd;
int cpuDispatchFd;
int kernelDispatchFd;
int cpuInterruptFd;
int kernelInterruptFd;

// Threads & Mutex
bool interrupt = false;
pthread_mutex_t MUTEX_INTERRUPT;


int main(){

    inicializar_proceso();

    // Thread handler MEMORIA
    pthread_t THREAD_MEMORY;
    if (!pthread_create(&THREAD_MEMORY, NULL, (void*) handler_memory, NULL))
        pthread_detach(THREAD_MEMORY);
    else {
        cerrar_programa();
        return EXIT_FAILURE;
    }  
        
    // Thread en que escuchará los mensajes de INTERRUPT enviados por KERNEL
    pthread_t THREAD_INTERRUPT;
    if (!pthread_create(&THREAD_INTERRUPT, NULL, (void*) interrupt_server, NULL))
        pthread_detach(THREAD_INTERRUPT);
    else {
        cerrar_programa();
        return EXIT_FAILURE;
    }  

    // Thread en que escuchará los mensajes de dispatch enviados por KERNEL
    pthread_t THREAD_DISPATCH;
    if (!pthread_create(&THREAD_DISPATCH, NULL, (void*) dispatch_server, NULL))
        pthread_detach(THREAD_DISPATCH);
    else {
        cerrar_programa();
        return EXIT_FAILURE;
    }

    for(;;);

    pthread_join(THREAD_MEMORY, NULL);
    pthread_join(THREAD_DISPATCH, NULL);
    pthread_join(THREAD_INTERRUPT, NULL);

    cerrar_programa();
}
