#include "largo_plazo.h"

void handler_largo_plazo() {
    log_info(mainLog, "Controlador de Largo plazo iniciado correctamente");

    pthread_t THREAD_CONSOLA_LISTENER;
    if(!pthread_create(&THREAD_CONSOLA_LISTENER, NULL, (void*) handler_consola_listener, NULL))
        pthread_detach(THREAD_CONSOLA_LISTENER);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL LISTENER DE CONSOLA. ABORTANDO.");
        return EXIT_FAILURE;
    }

    pthread_t THREAD_NEW_TO_READY;
    if(!pthread_create(&THREAD_NEW_TO_READY, NULL, (void*) handler_new_to_ready, NULL))
        pthread_detach(THREAD_NEW_TO_READY);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE NEW TO READY. ABORTANDO.");
        return EXIT_FAILURE;
    }

    pthread_t THREAD_EXIT;
    if(!pthread_create(&THREAD_EXIT, NULL, (void*) handler_exit, NULL))
        pthread_detach(THREAD_EXIT);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE EXIT. ABORTANDO.");
        return EXIT_FAILURE;
    }
}

// Escucha a consola y agrega a las mismas a la cola de NEW
void handler_consola_listener(){
    log_info(mainLog, "Controlador de consola iniciado correctamente");
    while(server_escuchar(SERVERNAME, server_fd));
}

// Dependiendo del nivel de multiprogramacion pasa de new a ready. Se lo comunica a memoria
void handler_new_to_ready(){
    log_info(mainLog, "Controlador de new to ready done");

}

// Si hay un proceso en la cola de exit. Se encarga de sacarlo y notificarle a memoria y a la consola
void handler_exit(){
    log_info(mainLog, "Controlador de exit done");

}