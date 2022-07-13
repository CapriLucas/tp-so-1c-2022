#include "dispatch.h"

// Fetch instruction
t_instruccion* fetch_instruction (t_PCB* pcb) {

    // t_instruccion* instruc = malloc(sizeof(t_instruccion));    
    t_instruccion* instruc = list_get(pcb->instructions_list, pcb->pc);

    return instruc;
}

// Execute instruction
bool exec_instruction (t_PCB* pcb, t_instruccion* instruc) {

    bool desalojar = false;

    pcb->pc++;

    switch (instruc->codigo_instruccion) {
        
        case NO_OP:
            exec_no_op();
            break;
        case I_O:
            exec_i_o(pcb, instruc->param_1);    // param_1: tiempo de bloqueo en msec.
            desalojar = true;
            break;
        case READ:
            exec_read(pcb, instruc);
            break;
        case COPY:
            exec_copy(pcb, instruc);
            break;
        case WRITE:
            exec_write(pcb, instruc);
            break;
        case EXIT:
            exec_exit(pcb);
            desalojar = true;
            break;

    }

    // free(instruc);
    return desalojar;
}

// Dispatch server
void dispatch_server() {

    // Inicia server en puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    char* puerto_dispatch = string_itoa(config_CPU->PUERTO_ESCUCHA_DISPATCH);
    cpuDispatchFd = iniciar_servidor (
        log_CPU,
        "CPU",
        "127.0.0.1", 
        puerto_dispatch
    );

    if (!cpuDispatchFd) {
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de KERNEL para mensajes de dispatch
    kernelDispatchFd = esperar_cliente (
        log_CPU,
        "KERNEL DISPATCH", 
        cpuDispatchFd
    );

    if (!kernelDispatchFd) {
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    while (1) {
        t_paquete* paquete = malloc(sizeof(t_paquete));

        if(recibir_header(paquete, kernelDispatchFd) == 0){
            return;
        }
        if(paquete->codigo_operacion != MSG_EXEC){
            log_info(log_CPU, "Se espera recibir MSG_EXEC codigo de operacion");
            return;
        }
        t_PCB* pcb = malloc(sizeof(t_PCB));
        deserialize_msg_exec(paquete, pcb);
    
        size_t size_list = list_size(pcb->instructions_list);
        
        for(int i = pcb->pc; i < size_list; i++) {
            t_instruccion* instruc = fetch_instruction(pcb);
            // datos para copy = fetch_operands(instruc)
            
            bool desalojar = exec_instruction(pcb, instruc);
            if(desalojar) {
                break;
            }
            
            pthread_mutex_lock(&MUTEX_INTERRUPT);            
            if(interrupt) {
                interrupt = false;
                pthread_mutex_unlock(&MUTEX_INTERRUPT);
                return_pcb_interrupt(pcb);
                break;
            } else {
                pthread_mutex_unlock(&MUTEX_INTERRUPT);                
            }
        }

        destroy_pcb(pcb);
        eliminar_paquete(paquete);
    }
}


// Execute NO_OP
int exec_no_op () {

    log_info(log_CPU, "Ejecutando instrución NO_OP");
    
    log_info(log_CPU, "Iniciando usleep(%u)...", config_CPU->RETARDO_NOOP);
    usleep(config_CPU->RETARDO_NOOP * 1000);
    log_info(log_CPU, "Finalizando usleep(%u)...", config_CPU->RETARDO_NOOP);

    return EXIT_SUCCESS;

}


// Execute I/O
int exec_i_o (t_PCB* pcb, uint32_t msec) {
  
    log_info(log_CPU, "Ejecutando instrucción I/O %u", msec);

    t_paquete* paquete = serialize_msg_i_o(msec, pcb);
    
    enviar_paquete(paquete, kernelDispatchFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}


// Execute READ
int exec_read (t_PCB* pcb, t_instruccion* instruc) {

    log_info(log_CPU, "Ejecutando instrucción READ %u", instruc->param_1);
    send(memoriaFd, "READ", instruc->param_1, instruc->param_2);
    
    return EXIT_SUCCESS;
}


// Execute COPY
int exec_copy (t_PCB* pcb, t_instruccion* instruc) {

    log_info(log_CPU, "Ejecutando instrucción COPY %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "COPY", instruc->param_1, instruc->param_2);

    return EXIT_SUCCESS;

}


// Execute WRITE
int exec_write (t_PCB* pcb, t_instruccion* instruc) {

    log_info(log_CPU, "Ejecutando instrucción WRITE %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "WRITE", instruc->param_1, instruc->param_2);
    
    return EXIT_SUCCESS;

}


// Execute EXIT
int exec_exit (t_PCB* pcb) {

    log_info(log_CPU, "Ejecutando instrucción EXIT");  

    t_paquete* paquete = serialize_msg_exit(pcb);
    
    enviar_paquete(paquete, kernelDispatchFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}


// Execute INTERRUPT
int return_pcb_interrupt (t_PCB* pcb) {

    log_info(log_CPU, "Ejecutando INTERRUPT");  

    t_paquete* paquete = serialize_msg_interrupt_ack(pcb);
    
    enviar_paquete(paquete, kernelDispatchFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}

