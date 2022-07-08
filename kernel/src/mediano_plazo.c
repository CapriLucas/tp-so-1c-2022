#include "mediano_plazo.h"

void handler_check_blocked_timer();
void process_timer(t_PCB_BLOCKED* pcb_to_suspend);
void handler_susp_blocked_to_ready();

void handler_mediano_plazo(){

    pthread_t THREAD_CHECK_BLOCKED_TIMER;
    if(!pthread_create(&THREAD_CHECK_BLOCKED_TIMER, NULL, (void*) handler_check_blocked_timer, NULL))
        pthread_detach(THREAD_CHECK_BLOCKED_TIMER);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE THREAD_CHECK_BLOCKED_TIMER. ABORTANDO.");
        exit(EXIT_FAILURE);
    }

    pthread_t THREAD_SUSP_BLOCKED_TO_READY;
    if(!pthread_create(&THREAD_SUSP_BLOCKED_TO_READY, NULL, (void*) handler_susp_blocked_to_ready, NULL))
        pthread_detach(THREAD_SUSP_BLOCKED_TO_READY);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE THREAD_SUSP_BLOCKED_TO_READY. ABORTANDO.");
        exit(EXIT_FAILURE);
    }
}

void handler_susp_blocked_to_ready(){
    log_info(mainLog, "Controlador de \"suspended blocked to ready\" done");
    while(1){
        t_PCB_BLOCKED* pcb_blocked;
        sem_wait(&CONTADOR_LISTA_SUSPENDED_BLOCKED);
        sem_wait(&GRADO_MULTIPROGRAMACION);
        pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
            pcb_blocked = list_get(LISTA_SUSPENDED_BLOCKED, 0);
            list_remove(LISTA_SUSPENDED_BLOCKED, 0);
        pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_BLOCKED);

        //TODO enviar a memoria para crear proceso y despues agregarlo a READY

        pthread_mutex_lock(&MUTEX_LISTA_READY);
            list_add(LISTA_READY, pcb_blocked->pcb);
            sem_post(&LISTA_READY_INTERRUPT);
            sem_post(&CONTADOR_LISTA_READY);
        pthread_mutex_unlock(&MUTEX_LISTA_READY);

        log_info(mainLog,"PID: %d. A cola ready", pcb_blocked->pcb->pid);
    }
}

// TODO hacer refactor de esto. En vez de escuchar al semaforo deberia ser una
// Funcion que se llama cada vez que hacemos un list_add(LISTA_BLOCKED). Solo recibiria el pid

void handler_check_blocked_timer(){
    while(1){
        // Obtenemos el ultimo PCB agregado y levantamos su hilo timer
        sem_wait(&LISTA_BLOCKED_TIMER);
        pthread_mutex_lock(&MUTEX_LISTA_BLOCKED);
            int size = list_size(LISTA_BLOCKED);
            t_PCB_BLOCKED* pcb_blocked;
            if(size > 0){
                pcb_blocked = list_get(LISTA_BLOCKED, size - 1);
            } else {
                pcb_blocked = NULL;
            }
        pthread_mutex_unlock(&MUTEX_LISTA_BLOCKED);
        if(pcb_blocked != NULL){
            pthread_t THREAD_PROCESS_TIMER;
            if(!pthread_create(&THREAD_PROCESS_TIMER, NULL, (void*) process_timer, pcb_blocked))
                pthread_detach(THREAD_PROCESS_TIMER);
            else {
                log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE THREAD_PROCESS_TIMER. ABORTANDO.");
                exit(EXIT_FAILURE);
            }
        }
    }
}


void process_timer(t_PCB_BLOCKED* pcb_to_suspend){
    uint32_t pid_to_suspend = pcb_to_suspend->pcb->pid;
    bool _is_the_pcb_to_suspend(t_PCB_BLOCKED* pcb_blocked) {
        bool res = pcb_blocked->pcb->pid == pid_to_suspend;
        return res;
    }
    usleep(mainConfig->TIEMPO_MAXIMO_BLOQUEADO);
    pthread_mutex_lock(&MUTEX_LISTA_BLOCKED);
        t_PCB_BLOCKED* pcb_blocked = list_find(LISTA_BLOCKED, (void*) _is_the_pcb_to_suspend);
    pthread_mutex_unlock(&MUTEX_LISTA_BLOCKED);
    if(pcb_blocked != NULL){
        sem_wait(&CONTADOR_LISTA_BLOCKED);
        pthread_mutex_lock(&MUTEX_LISTA_BLOCKED);
            list_remove_by_condition(LISTA_BLOCKED,(void*) _is_the_pcb_to_suspend);
        pthread_mutex_unlock(&MUTEX_LISTA_BLOCKED);
        pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
            list_add(LISTA_SUSPENDED_BLOCKED, pcb_blocked);
        pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
        sem_post(&CONTADOR_LISTA_SUSPENDED_BLOCKED);
        sem_post(&GRADO_MULTIPROGRAMACION);
        log_info(mainLog, "Suspendemos al proceso PID:%d", pid_to_suspend);
    }
}
