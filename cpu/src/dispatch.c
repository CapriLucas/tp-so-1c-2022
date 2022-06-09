#include "dispatch.h"


// Execute NO_OP
int exec_no_op (t_PCB* pcb) {

    uint32_t p_1 = 5;

    log_info(log_CPU, "Ejecutando instrución NO_OP %u", p_1);

    uint32_t ms = config_CPU->RETARDO_NOOP * p_1;
    
    log_info(log_CPU, "Iniciando usleep(%u)...", ms);
    usleep(ms);
    log_info(log_CPU, "Finalizando usleep(%u)...", ms);

    return EXIT_SUCCESS;

}


// Execute I/O
int exec_i_o (t_PCB* pcb) {

    uint32_t p_1 = 3000;
    
    log_info(log_CPU, "Ejecutando instrucción I/O %u", p_1);
    send(kernelDispatchFd, "I/O", 4, 0);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd);

    return EXIT_SUCCESS;

}


// Execute READ
int exec_read (t_PCB* pcb) {

    uint32_t p_1 = 0;

    log_info(log_CPU, "Ejecutando instrucción READ %u", p_1);
    send(memoriaFd, "READ", 5, 0);
    printf("memoriaFd: %d\n", memoriaFd);
    
    return EXIT_SUCCESS;
}


// Execute COPY
int exec_copy (t_PCB* pcb) {

    uint32_t p_1 = 4;
    uint32_t p_2 = 42;

    log_info(log_CPU, "Ejecutando instrucción COPY %u %u", p_1, p_2);
    send(memoriaFd, "COPY", 5, 0);
    printf("memoriaFd: %d\n", memoriaFd);

    return EXIT_SUCCESS;

}


// Execute WRITE
int exec_write (t_PCB* pcb) {

    uint32_t p_1 = 4;
    uint32_t p_2 = 42;

    log_info(log_CPU, "Ejecutando instrucción WRITE %u %u", p_1, p_2);
    send(memoriaFd, "WRITE", 6, 0);
    printf("memoriaFd: %d\n", memoriaFd);
    
    return EXIT_SUCCESS;

}


// Execute EXIT
int exec_exit (t_PCB* pcb) {

    log_info(log_CPU, "Ejecutando instrucción EXIT");  
    send(kernelDispatchFd, "EXIT", 5, 0);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd);

    return EXIT_SUCCESS;

}

