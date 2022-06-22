#include "dispatch.h"


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

    // Size = Header + Milisegundos I/O + PCB + lista de instrucciones
    size_t size = sizeof(uint8_t) + sizeof(uint32_t) * 2 + sizeof(t_PCB) + sizeof(t_instruc) * list_size(pcb->l_instruc);

    void* stream = serialize_msg_i_o(msec, pcb);

    send(kernelDispatchFd, stream, size, 0);

    free(stream);

    return EXIT_SUCCESS;

}


// Execute READ
int exec_read (t_PCB* pcb, t_instruc* instruc) {

    log_info(log_CPU, "Ejecutando instrucción READ %u", instruc->param_1);
    send(memoriaFd, "READ", instruc->param_1, instruc->param_2);
    printf("memoriaFd: %d\n", memoriaFd);
    
    return EXIT_SUCCESS;
}


// Execute COPY
int exec_copy (t_PCB* pcb, t_instruc* instruc) {

    log_info(log_CPU, "Ejecutando instrucción COPY %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "COPY", instruc->param_1, instruc->param_2);
    printf("memoriaFd: %d\n", memoriaFd);

    return EXIT_SUCCESS;

}


// Execute WRITE
int exec_write (t_PCB* pcb, t_instruc* instruc) {

    log_info(log_CPU, "Ejecutando instrucción WRITE %u %u", instruc->param_1, instruc->param_2);
    send(memoriaFd, "WRITE", instruc->param_1, instruc->param_2);
    printf("memoriaFd: %d\n", memoriaFd);
    
    return EXIT_SUCCESS;

}


// Execute EXIT
int exec_exit (t_PCB* pcb) {

    log_info(log_CPU, "Ejecutando instrucción EXIT");  

    void* stream = serialize_msg_exit(pcb);

    // Size = Header + PCB + lista de instrucciones
    size_t size = sizeof(uint8_t) + sizeof(uint32_t) + sizeof(t_PCB) + sizeof(t_instruc) * list_size(pcb->l_instruc);

    send(kernelDispatchFd, stream, size, 0);

    free(stream);

    return EXIT_SUCCESS;

}

