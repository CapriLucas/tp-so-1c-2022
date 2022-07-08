#include "i_o.h"

void handler_i_o() {
    while(1){
        pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
        bool anySuspended = list_size(LISTA_SUSPENDED_BLOCKED) > 0;
        pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
        if(anySuspended){
            sem_wait(&CONTADOR_LISTA_SUSPENDED_BLOCKED);
            t_PCB_BLOCKED* pcb_blocked;
            pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_BLOCKED);
                pcb_blocked = list_get(LISTA_SUSPENDED_BLOCKED, 0);
                list_remove(LISTA_SUSPENDED_BLOCKED, 0);
            pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_BLOCKED);

            usleep(pcb_blocked->msec);
            
            log_info(mainLog, "DESBLOQUEO PID %d (suspended)", pcb_blocked->pcb->pid);
            pthread_mutex_lock(&MUTEX_LISTA_SUSPENDED_READY);
                list_add(LISTA_SUSPENDED_READY, pcb_blocked->pcb);
            pthread_mutex_unlock(&MUTEX_LISTA_SUSPENDED_READY);
            sem_post(&CONTADOR_LISTA_SUSPENDED_READY);
            free(pcb_blocked);
        } else {
            sem_wait(&CONTADOR_LISTA_BLOCKED);
            t_PCB_BLOCKED* pcb_blocked;
            pthread_mutex_lock(&MUTEX_LISTA_BLOCKED);
                pcb_blocked = list_get(LISTA_BLOCKED, 0);
                list_remove(LISTA_BLOCKED, 0);
            pthread_mutex_unlock(&MUTEX_LISTA_BLOCKED);
            
            usleep(pcb_blocked->msec);

            log_info(mainLog, "DESBLOQUEO PID %d", pcb_blocked->pcb->pid);
            pthread_mutex_lock(&MUTEX_LISTA_READY);
                list_add(LISTA_READY, pcb_blocked->pcb);
            pthread_mutex_unlock(&MUTEX_LISTA_READY);
            sem_post(&CONTADOR_LISTA_READY);
            sem_post(&LISTA_READY_INTERRUPT);
            free(pcb_blocked);
        }
    }
}