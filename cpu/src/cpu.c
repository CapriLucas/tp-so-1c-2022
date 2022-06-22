#include "cpu.h"


int fetch_instruction (t_PCB* pcb) {    // Cambiar por execute_instruction

    t_instruc* instruc = malloc(sizeof(t_instruc));    
    instruc = list_get(pcb->l_instruc, pcb->pc);
    pcb->pc++;
    // Testear PC aumenta
    switch (instruc->instruc_cod) {
        
        case NO_OP:
            exec_no_op();
            break;
        case I_O:
            exec_i_o(pcb, instruc->param_1);    // param_1: tiempo de bloqueo en msec.
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
            break;

    }

    free(instruc);
    return EXIT_SUCCESS;
}


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
        "KERNEL", 
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

    pcb->l_instruc = list_create();

    // NO_OP
    t_instruc* i_0 = malloc(sizeof(t_instruc));
    i_0->instruc_cod = NO_OP;
    i_0->param_1 = 5;
    i_0->param_2 = 987654321;
    list_add(pcb->l_instruc, i_0);
    // I_O
    t_instruc* i_1 = malloc(sizeof(t_instruc));
    i_1->instruc_cod = I_O;
    i_1->param_1 = 3000;
    i_1->param_2 = 123456789;
    list_add(pcb->l_instruc, i_1);
    // READ
    t_instruc* i_2 = malloc(sizeof(t_instruc));
    i_2->instruc_cod = READ;
    i_2->param_1 = 10;
    i_2->param_2 = 110;
    list_add(pcb->l_instruc, i_2);
    // COPY
    t_instruc* i_3 = malloc(sizeof(t_instruc));
    i_3->instruc_cod = COPY;
    i_3->param_1 = 0;
    i_3->param_2 = 4;
    list_add(pcb->l_instruc, i_3);
    // WRITE
    t_instruc* i_4 = malloc(sizeof(t_instruc));
    i_4->instruc_cod = WRITE;
    i_4->param_1 = 4;
    i_4->param_2 = 42;  
    list_add(pcb->l_instruc, i_4);
    //EXIT
    t_instruc* i_5 = malloc(sizeof(t_instruc));
    i_5->instruc_cod = EXIT;
    i_5->param_1 = 0;
    i_5->param_2 = 0;  
    list_add(pcb->l_instruc, i_5);
    //

    /*
    
    fetch input: PCB; output: t_instruc 

    */
 
    int c = list_size(pcb->l_instruc);
    printf("List size: %d\n", c);
    for(int i = 0; i < c; i++) {
        // t_instruc* instruc = fetch_instruction(pcb);
        // datos para copy = fetch_operands(instruc)
        // bool is_
     /*    bool desalojar = execute_instruction(instruc);  // execute_instruction pc++
        if(desalojar) {
            break;
        }
        if(interrupt) {
            return_pcb(pcb); // o return_interrupt(pcb) ...
            break;
        } */
        fetch_instruction(pcb);
    }

}


void interrupt_server() {

    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    cpuInterruptFd = iniciar_servidor (
        log_CPU,
        "CPU",
        "127.0.0.1", 
        "8005"  // config_CPU->PUERTO_ESCUCHA_INTERRUPT
    );

    if (!cpuInterruptFd) {    
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de KERNEL para mensajes de interrupciones
    kernelInterruptFd = esperar_cliente (
        log_CPU, 
        "KERNEL", 
        cpuInterruptFd
    );

    if (!kernelInterruptFd) {    
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

/* 
    // Test recv PCB from Kernel -- Borrar

    void* header_test = malloc(sizeof(uint8_t) + sizeof(uint32_t));
    uint8_t message_test;
    uint32_t payload_size_test;
    recv(kernelDispatchFd, header_test, sizeof(uint8_t) + sizeof(uint32_t), 0);
    deserialize_header(header_test, &message_test, &payload_size_test);
    free(header_test);

    printf("Header post-serialización:\n");
    printf("header->message %u\n", message_test);
    printf("header->payload_size %u\n", payload_size_test);

    // Recibe y deserializa payload
    t_PCB* pcb_test = malloc(sizeof(t_PCB));
    void* stream_test = malloc(payload_size_test);
    recv(kernelDispatchFd, stream_test, payload_size_test, 0);
    deserialize_msg_exit(stream_test, payload_size_test, pcb_test);
    free(stream_test);

    printf("\nPCB post-serialización\n");
    printf("pcb->pid: %u\n", pcb_test->pid);
    printf("pcb_aux->process_size: %u\n", pcb_test->process_size);
    printf("pcb_aux->pc: %u\n", pcb_test->pc);
    printf("pcb_aux->page_table_id: %u\n", pcb_test->page_table_id);
    printf("pcb_aux->burst_prediction: %u\n\n", pcb_test->burst_prediction);

    printf("\nInstruc post-serialización\n");
    t_instruc* instruc_test = malloc(sizeof(t_instruc));
    size_t size_list_test = list_size(pcb_test->l_instruc);
    for (int i = 0; i < size_list_test; i++) {
        printf("Dentro del for\n");
        instruc_test = list_get(pcb_test->l_instruc, i);
        printf("instruc_cod: %u\n", instruc_test->instruc_cod);
        printf("param_1: %u\n", instruc_test->param_1);
        printf("param_2: %u\n", instruc_test->param_2);
    }

 */
    //

/*     // Test thread & interrupt message from kernel. -- Borrar
    printf("Estoy dentro del hilo interrupt.\n");
    char* buffer = malloc(15);
    int recv_bytes = recv(kernelInterruptFd, buffer, 14, 0);
    buffer[recv_bytes] = '\0';
    printf("Mensaje: %s\n", buffer);
    free(buffer); */

}



int main(){

    inicializar_proceso();

    // Test load config_CPU -- Borrar
    printf("ENTRADAS_TLB: %u\n", config_CPU->ENTRADAS_TLB);
    printf("REEMPLAZO_TLB: %s\n", config_CPU->REEMPLAZO_TLB);
    printf("RETARDO_NOOP: %u\n", config_CPU->RETARDO_NOOP);
    printf("IP_MEMORIA: %s\n", config_CPU->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %u\n", config_CPU->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA_DISPATCH: %u\n", config_CPU->PUERTO_ESCUCHA_DISPATCH);
    printf("PUERTO_ESCUCHA_INTERRUPT: %u\n", config_CPU->PUERTO_ESCUCHA_INTERRUPT); 
    
    // Crear conexión con MEMORIA 
    memoriaFd = crear_conexion (
        log_CPU, 
        "CPU", 
        config_CPU->IP_MEMORIA, 
        "8002" // config_CPU->PUERTO_MEMORIA
    );

    if (!memoriaFd) {        
        cerrar_programa();
        return EXIT_FAILURE;
    }

    // Thread en que escuchará los mensajes de INTERRUPT enviados por KERNEL
    pthread_t THREAD_INTERRUPT;
    if (!pthread_create(&THREAD_INTERRUPT, NULL, (void*) interrupt_server, NULL))
        pthread_detach(THREAD_INTERRUPT);
    else {
        cerrar_programa();
        return EXIT_FAILURE;
    }  

    // Thread en que escuchará los mensajes de dispatch enviados por KERNEL
    pthread_t THREAD_DISPATCH;
    if (!pthread_create(&THREAD_DISPATCH, NULL, (void*) dispatch_server, NULL))
        pthread_detach(THREAD_DISPATCH);
    else {
        cerrar_programa();
        return EXIT_FAILURE;
    }

    for(;;);

    pthread_join(THREAD_DISPATCH, NULL);
    pthread_join(THREAD_INTERRUPT, NULL);

    cerrar_programa();
}
