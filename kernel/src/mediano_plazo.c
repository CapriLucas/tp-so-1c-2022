#include "mediano_plazo.h"

void handler_check_blocked_timer(t_PCB_BLOCKED* pcb_to_suspend);
void process_timer(uint32_t* pid_to_suspend);
void handler_susp_ready_to_ready();

void handler_mediano_plazo(){

    pthread_t THREAD_SUSP_READY_TO_READY;
    if(!pthread_create(&THREAD_SUSP_READY_TO_READY, NULL, (void*) handler_susp_ready_to_ready, NULL))
        pthread_detach(THREAD_SUSP_READY_TO_READY);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE THREAD_SUSP_READY_TO_READY. ABORTANDO.");
        exit(EXIT_FAILURE);
    }
}

void handler_susp_ready_to_ready(){
    log_info(mainLog, "Controlador de \"suspended ready to ready\" done");
    while(1){
        t_PCB* pcb;
        sem_wait(&CONTADOR_LISTA_SUSPENDED_READY);
        sem_wait(&GRADO_MULTIPROGRAMACION);
        pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_READY);
            pcb = list_get(LISTA_SUSPENDED_READY, 0);
            list_remove(LISTA_SUSPENDED_READY, 0);
        pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_READY);

        //TODO enviar a memoria para crear proceso y despues agregarlo a READY

        pthread_mutex_lock(&MUTEX_LISTA_READY);
            list_add(LISTA_READY, pcb);
            sem_post(&LISTA_READY_INTERRUPT);
            sem_post(&CONTADOR_LISTA_READY);
        pthread_mutex_unlock(&MUTEX_LISTA_READY);

        log_info(mainLog,"PID: %d. A cola ready", pcb->pid);
    }
}

void handler_check_blocked_timer(t_PCB_BLOCKED* pcb_to_suspend){
    uint32_t* pid_to_suspend = malloc(sizeof(uint32_t));
    uint32_t aux = pcb_to_suspend->pcb->pid;
    memcpy(pid_to_suspend, &(aux), sizeof(uint32_t));

    pthread_t THREAD_PROCESS_TIMER;
    if(!pthread_create(&THREAD_PROCESS_TIMER, NULL, (void*) process_timer, pid_to_suspend))
        pthread_detach(THREAD_PROCESS_TIMER);
    else {
        log_error(mainLog, "ERROR CRITICO INICIANDO EL CONTROLADOR DE THREAD_PROCESS_TIMER. ABORTANDO.");
        exit(EXIT_FAILURE);
    }
}


void process_timer(uint32_t* pid_to_suspend){
    bool _is_the_pcb_to_suspend(t_PCB_BLOCKED* pcb_blocked) {
        bool res = pcb_blocked->pcb->pid == *pid_to_suspend;
        return res;
    }
    usleep(mainConfig->TIEMPO_MAXIMO_BLOQUEADO * 1000);
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
        log_info(mainLog, "Suspendemos al proceso PID:%d", *pid_to_suspend);
    }
    free(pid_to_suspend);
}
