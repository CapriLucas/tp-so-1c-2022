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
        sem_wait(&CONTADOR_LISTA_READY);
        pthread_mutex_lock(&MUTEX_LISTA_READY);
            t_PCB* pcb = get_next_pcb_to_exec();
        pthread_mutex_unlock(&MUTEX_LISTA_READY);

        t_paquete* mensaje_para_cpu = serialize_msg_exec(pcb);
        enviar_paquete(mensaje_para_cpu, kernelDispatchFd);
        log_info(mainLog, "Ejecutamos PID: %d",pcb->pid);
        destroy_pcb(pcb);
        eliminar_paquete(mensaje_para_cpu);
        
        t_paquete* paquete = malloc(sizeof(t_paquete));
        t_PCB* pcb_recibido = malloc(sizeof(t_PCB));

        if(recibir_header(paquete, kernelDispatchFd) == 0){
        return;
        }

        uint32_t msec; 
        switch(paquete->codigo_operacion){
            case MSG_I_O:
                deserialize_msg_i_o(paquete, &msec, pcb_recibido);
                t_PCB_BLOCKED* pcb_blocked = malloc(sizeof(t_PCB_BLOCKED));
                pcb_blocked->msec = msec;
                pcb_blocked->pcb = pcb_recibido;
                pthread_mutex_lock(&MUTEX_LISTA_BLOCKED);
                    list_add(LISTA_BLOCKED, pcb_blocked);
                pthread_mutex_unlock(&MUTEX_LISTA_BLOCKED);
                //Hacemos el post de contador_lista_blocked una vez que ya leimos el contenido en el timer
                handler_check_blocked_timer(pcb_blocked);
                log_info(mainLog, "Se bloquea pid: %d", pcb_recibido->pid);
                break;
            case MSG_EXIT:
                deserialize_msg_exit(paquete, pcb_recibido);
                pthread_mutex_lock(&MUTEX_LISTA_EXIT);
                    list_add(LISTA_EXIT, pcb_recibido);
                    sem_post(&CONTADOR_LISTA_EXIT);
                pthread_mutex_unlock(&MUTEX_LISTA_EXIT);
                sem_post(&GRADO_MULTIPROGRAMACION);
                break;
            case MSG_INTERRUPT_ACK:
                deserialize_msg_interrupt_ack(paquete, pcb_recibido);
                pthread_mutex_lock(&MUTEX_LISTA_READY);
                    list_add(LISTA_READY, pcb_recibido);
                    sem_post(&CONTADOR_LISTA_READY);
                pthread_mutex_unlock(&MUTEX_LISTA_READY);
                break;
            default:
                log_info(mainLog, "ERROR");
        }

        eliminar_paquete(paquete);
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
        sem_wait(&LISTA_READY_INTERRUPT);
        t_paquete* paquete = serialize_msg_interrupt();
        enviar_paquete(paquete, kernelInterruptFd);
        eliminar_paquete(paquete);
    }
}