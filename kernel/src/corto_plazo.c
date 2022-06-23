#include "corto_plazo.h"

void handler_corto_plazo() {
    log_info(mainLog, "Controlador de corto plazo iniciado correctamente");

    pthread_t THREAD_CICLO_CORTO_PLAZO;
    if(!pthread_create(&THREAD_CICLO_CORTO_PLAZO, NULL, (void*) handler_ciclo_corto_plazo, NULL))
        pthread_detach(THREAD_CICLO_CORTO_PLAZO);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL handler del ciclo de corto plazo. ABORTANDO.");
        exit(EXIT_FAILURE);
    }

    if(false){//TODO poner: si no es SRT. O si es fifo
        pthread_t THREAD_CHECK_READY_LIST;
        if(!pthread_create(&THREAD_CHECK_READY_LIST, NULL, (void*) handler_check_ready_list, NULL))
            pthread_detach(THREAD_CHECK_READY_LIST);
        else {
            log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE check ready list. ABORTANDO.");
            exit(EXIT_FAILURE);
        }
    }
}

void handler_ciclo_corto_plazo(){
    log_info(mainLog, "Controlador de ciclo de corto plazo iniciado correctamente");

    // Crear conexión con CPU (dispatch)  
    kernelDispatchFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8001"  // mainConfig->PUERTO_CPU_DISPATCH 
    );

    while(1){
        //Sacamos de ready el pcb
        t_PCB* pcb;
        sem_wait(&CONTADOR_LISTA_READY);
        pthread_mutex_lock(&MUTEX_LISTA_READY);
            // Ver si sacarlo por fifo o SRT
            pcb = list_get(LISTA_NEW, 0);
            list_remove(LISTA_NEW, 0);
        pthread_mutex_unlock(&MUTEX_LISTA_READY);

        // Enviamos el pcb al CPU
        // Hacer free del pcb
        // Nos quedamos en recv esperando el PCB de CPU
    
        // Decidimos con el pcb que recibimos desde CPU si agregarlo a EXIT, READY, BLOCKED
    }
}

void handler_check_ready_list(){
    log_info(mainLog, "Controlador de check ready list iniciado correctamente");
    
    // Crear conexión con CPU (interrupciones)
    kernelInterruptFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_CPU, 
        "8005"  // mainConfig->PUERTO_CPU_INTERRUPT
    );

    while(1){
        // Escuchar si llega un pcb a la lista READY
        // then
        // Enviar mensaje a CPU de interrupcion y listo
    }
}