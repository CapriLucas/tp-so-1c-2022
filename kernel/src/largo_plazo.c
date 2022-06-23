#include "largo_plazo.h"

void handler_largo_plazo() {
    log_info(mainLog, "Controlador de Largo plazo iniciado correctamente");

    pthread_t THREAD_CONSOLA_LISTENER;
    if(!pthread_create(&THREAD_CONSOLA_LISTENER, NULL, (void*) handler_consola_listener, NULL))
        pthread_detach(THREAD_CONSOLA_LISTENER);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL LISTENER DE CONSOLA. ABORTANDO.");
        exit(EXIT_FAILURE);
    }

    pthread_t THREAD_NEW_TO_READY;
    if(!pthread_create(&THREAD_NEW_TO_READY, NULL, (void*) handler_new_to_ready, NULL))
        pthread_detach(THREAD_NEW_TO_READY);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE NEW TO READY. ABORTANDO.");
        exit(EXIT_FAILURE);
    }

    pthread_t THREAD_EXIT;
    if(!pthread_create(&THREAD_EXIT, NULL, (void*) handler_exit, NULL))
        pthread_detach(THREAD_EXIT);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE EXIT. ABORTANDO.");
        exit(EXIT_FAILURE);
    }
}

// Escucha a consola y agrega a las mismas a la cola de NEW
// Bloqueado por: RECV de consola
// Libera: 1 + en items NEW
void handler_consola_listener(){
    log_info(mainLog, "Controlador de consola iniciado correctamente");
    while(server_escuchar(SERVERNAME, server_fd));
}

// Dependiendo del nivel de multiprogramacion pasa de new a ready. Se lo comunica a memoria
// Bloqueado por: Cantidad de items en NEW, multiprogramacion disponible
// Libera: 1 + en items READY
void handler_new_to_ready(){
    log_info(mainLog, "Controlador de \"new to ready\" done");
    while(1){
        t_PCB* pcb;
        sem_wait(&CONTADOR_LISTA_NEW);
        sem_wait(&GRADO_MULTIPROGRAMACION);
        pthread_mutex_lock(&MUTEX_LISTA_NEW);
            pcb = list_get(LISTA_NEW, 0);
            list_remove(LISTA_NEW, 0);
        pthread_mutex_unlock(&MUTEX_LISTA_NEW);
        //TODO enviar a memoria para crear proceso y despues agregarlo a READY
        pthread_mutex_lock(&MUTEX_LISTA_READY);
            list_add(LISTA_READY, pcb);
            sem_post(&CONTADOR_LISTA_READY);
        pthread_mutex_unlock(&MUTEX_LISTA_READY);

        log_info(mainLog,"El proceso (%d) fue anadido a la cola ready", pcb->pid);
    }
}


// Si hay un proceso en la cola de exit. Se encarga de sacarlo y notificarle a memoria y a la consola
 t_PCB* pcb_exit;

bool _is_the_pcb_to_delete(t_socket_pid *p) {
    bool res = pcb_exit->pid == p->pid;
    return res;
}

void handler_exit(){
    log_info(mainLog, "Controlador de exit done");
     while(1){

//TODO EN CUANTO ESTE EL PLANIFICADOR CORTO PLAZO SACAR ESTO PORFAVOR
// //EMULADOR PARA PROBAR pasar de ready a exit apenas vayan llegando
//         t_PCB* pcb;
//         sem_wait(&CONTADOR_LISTA_READY);
//         pthread_mutex_lock(&MUTEX_LISTA_READY);
//             pcb = list_get(LISTA_READY, 0);
//             list_remove(LISTA_READY, 0);
//         pthread_mutex_unlock(&MUTEX_LISTA_READY);
//         pthread_mutex_lock(&MUTEX_LISTA_EXIT);
//             list_add(LISTA_EXIT, pcb);
//             log_info(mainLog,"Agregamos a exit el proceso (%d)",pcb->pid);
//             sem_post(&CONTADOR_LISTA_EXIT);
//         pthread_mutex_unlock(&MUTEX_LISTA_EXIT);  
// // ACA EMPIEZA

        sem_wait(&CONTADOR_LISTA_EXIT);
        pthread_mutex_lock(&MUTEX_LISTA_EXIT);
            pcb_exit = list_get(LISTA_EXIT, 0);
            list_remove(LISTA_EXIT, 0);
            log_info(mainLog,"obtenemos de exit el proceso (%d)",pcb_exit->pid);
        pthread_mutex_unlock(&MUTEX_LISTA_EXIT);

        pthread_mutex_lock(&MUTEX_LISTA_EXIT_PID);
            t_socket_pid* socket_pid = list_find(LISTA_EXIT_PID, (void*) _is_the_pcb_to_delete);
            list_remove_by_condition(LISTA_EXIT_PID,(void*) _is_the_pcb_to_delete);
        pthread_mutex_unlock(&MUTEX_LISTA_EXIT_PID);

        int socket_consola = socket_pid->console_fd;

        send(socket_consola, "EXIT", 4, 0);
        //TODO Mandar senal para matar proceso en memoria;
        free(socket_pid);
        free(pcb_exit);
        liberar_conexion(&socket_consola);

    }
}