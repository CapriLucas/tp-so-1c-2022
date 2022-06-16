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
int exec_i_o (t_PCB* pcb, uint32_t lock_time) {
  
    log_info(log_CPU, "Ejecutando instrucción I/O %u", lock_time);

    void* stream = serialize_pcb(pcb);

    send(kernelDispatchFd, stream, 60, 0);

    free(stream);

/*     t_paquete* paquete = crear_paquete(ENVIAR_PSEUDO_CODIGO);   // ToDo: Modificar código operación
    agregar_a_paquete(paquete, (void*) pcb, sizeof(t_PCB));
    enviar_paquete(paquete, kernelDispatchFd);
    eliminar_paquete(paquete); */

/*     send(kernelDispatchFd, "I/O", 4, NULL);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd); */

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
    send(kernelDispatchFd, "EXIT", 5, 0);
    printf("kernelDispatchFd: %d\n", kernelDispatchFd);

    return EXIT_SUCCESS;

}

