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


uint32_t crear_proceso_en_memoria(uint32_t pid,uint32_t process_size){
    t_creacion_memoria* mensaje = malloc(sizeof(t_creacion_memoria));
    mensaje->pid = pid;
    mensaje->process_size = process_size;
	t_paquete* paquete = crear_paquete(CREAR_PROCESO_EN_MEMORIA);
    agregar_a_paquete(paquete, mensaje, sizeof(t_creacion_memoria));
    //El paquete va a enviar un buffer de size 12
    enviar_paquete(paquete, memoriaFd);
    uint32_t page_table_id;
    recv(memoriaFd, &page_table_id, sizeof(uint32_t), MSG_WAITALL);

    free(mensaje);
    eliminar_paquete(paquete);
    return page_table_id;
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

        uint32_t page_table_id = crear_proceso_en_memoria(pcb->pid, pcb->process_size);
        log_info(mainLog,"ACAAA %d",page_table_id);
        //Enviar y recibir valor de tabla de paginas y ponerlo en el pcb
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

static void instructions_destroy(t_instruccion *self) {
    free(self);
} //TODO refactor esto a shared

void handler_exit(){
    log_info(mainLog, "Controlador de exit done");
     while(1){

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
        list_destroy_and_destroy_elements(pcb_exit->instructions_list, (void*) instructions_destroy);
        free(pcb_exit);
        liberar_conexion(&socket_consola);

    }
}