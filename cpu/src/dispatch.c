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
    cpuDispatchFd = iniciar_servidor (
        log_CPU,
        "CPU",
        "127.0.0.1", 
        "8001"  // config_CPU->PUERTO_ESCUCHA_DISPATCH
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

    // while (1)  { recv(PCB) hasta final de for

    // PCB de prueba --Borrar
    t_PCB* pcb = malloc(sizeof(t_PCB));

    pcb->pid = 123;
    pcb->process_size = 64;
    pcb->pc = 0;
    pcb->page_table_id = 16;
    pcb->burst_prediction = 3;

    pcb->instructions_list = list_create();

    // NO_OP
    t_instruccion* i_0 = malloc(sizeof(t_instruccion));
    i_0->codigo_instruccion = NO_OP;
    i_0->param_1 = 5;
    i_0->param_2 = 987654321;
    list_add(pcb->instructions_list, i_0);
    // READ
    t_instruccion* i_2 = malloc(sizeof(t_instruccion));
    i_2->codigo_instruccion = READ;
    i_2->param_1 = 10;
    i_2->param_2 = 110;
    list_add(pcb->instructions_list, i_2);
    // COPY
    t_instruccion* i_3 = malloc(sizeof(t_instruccion));
    i_3->codigo_instruccion = COPY;
    i_3->param_1 = 0;
    i_3->param_2 = 4;
    list_add(pcb->instructions_list, i_3);
    // WRITE
    t_instruccion* i_4 = malloc(sizeof(t_instruccion));
    i_4->codigo_instruccion = WRITE;
    i_4->param_1 = 4;
    i_4->param_2 = 42;  
    list_add(pcb->instructions_list, i_4);
     //EXIT
    t_instruccion* i_5 = malloc(sizeof(t_instruccion));
    i_5->codigo_instruccion = EXIT;
    i_5->param_1 = 0;
    i_5->param_2 = 0;  
    list_add(pcb->instructions_list, i_5);
    // I_O
    t_instruccion* i_1 = malloc(sizeof(t_instruccion));
    i_1->codigo_instruccion = I_O;
    i_1->param_1 = 3000;
    i_1->param_2 = 123456789;
    list_add(pcb->instructions_list, i_1);
    //

 
    size_t size_list = list_size(pcb->instructions_list);
        
    for(int i = pcb->pc; i < size_list; i++) {

        t_instruccion* instruc = fetch_instruction(pcb);
        
        bool desalojar = exec_instruction(pcb, instruc);
        if(desalojar) {
            break;
        }
        /*
        if(interrupt) {
            return_pcb(pcb); // o return_interrupt(pcb) ...
            break;
        } */
        // datos para copy = fetch_operands(instruc)
    }

}


// Execute NO_OP
int exec_no_op () {

    log_info(log_CPU, "Ejecutando instrución NO_OP");
    
    log_info(log_CPU, "Iniciando usleep(%u)...", config_CPU->RETARDO_NOOP);
    usleep(config_CPU->RETARDO_NOOP);
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
    printf("memoriaFd: %d\n", memoriaFd);
    
    return EXIT_SUCCESS;
}


// Execute COPY
int exec_copy (t_PCB* pcb, t_instruccion* instruc) {

    log_info(log_CPU, "Ejecutando instrucción COPY %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "COPY", instruc->param_1, instruc->param_2);
    printf("memoriaFd: %d\n", memoriaFd);

    return EXIT_SUCCESS;

}


// Execute WRITE
int exec_write (t_PCB* pcb, t_instruccion* instruc) {

    log_info(log_CPU, "Ejecutando instrucción WRITE %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "WRITE", instruc->param_1, instruc->param_2);
    printf("memoriaFd: %d\n", memoriaFd);
    
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

