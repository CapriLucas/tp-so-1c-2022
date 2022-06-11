#include "dispatch.h"


// Execute NO_OP
int exec_no_op (t_PCB* pcb, t_instruc* instruc) {

    log_info(log_CPU, "Ejecutando instrución NO_OP %u", instruc->param_1);

    uint32_t ms = config_CPU->RETARDO_NOOP * instruc->param_1;
    
    log_info(log_CPU, "Iniciando usleep(%u)...", ms);
    usleep(ms);
    log_info(log_CPU, "Finalizando usleep(%u)...", ms);

    return EXIT_SUCCESS;

}


// Execute I/O
int exec_i_o (t_PCB* pcb, t_instruc* instruc) {
  
    log_info(log_CPU, "Ejecutando instrucción I/O %u", instruc->param_1);
    send(kernelDispatchFd, "I/O", instruc->param_1, instruc->param_2);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd);

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
int exec_exit (t_PCB* pcb, t_instruc* instruc) {

    log_info(log_CPU, "Ejecutando instrucción EXIT");  
    send(kernelDispatchFd, "EXIT", instruc->param_1, instruc->param_2);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd);

    return EXIT_SUCCESS;

}

