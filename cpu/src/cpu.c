#include "cpu.h"

static void inicializar_proceso() {
    log_CPU = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    config_CPU = malloc(sizeof(t_config_CPU));
    if (!cargar_configuracion(config_CPU, log_CPU)) {
        exit(EXIT_FAILURE);
    }
}


int fetch_instruction (t_PCB* pcb) {

    pcb->pid = 10;
    pcb->process_size = 64;
    pcb->pc = 1;
    pcb->page_table_id = 16;
    pcb->burst_prediction = 3;

    // Borrar
    printf("PCB Process ID: %u\n", pcb->pid);
    printf("PCB Process size: %u\n", pcb->process_size);
    printf("PCB Program counter: %u\n", pcb->pc);
    printf("PCB Page table ID: %u\n", pcb->page_table_id);
    printf("PCB Burst prediction: %u\n", pcb->burst_prediction);
    //
    
    int instruction_code = 5;

    switch (instruction_code) {
        
        case NO_OP:
            exec_no_op(pcb);
            break;
        case I_O:
            exec_i_o(pcb);
            break;
        case READ:
            exec_read(pcb);
            break;
        case COPY:
            exec_copy(pcb);
            break;
        case WRITE:
            exec_write(pcb);
            break;
        case EXIT:
            exec_exit(pcb);
            break;

    }

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

    // Test thread & dispatch message from kernel. -- Borrar
    printf("Estoy dentro del hilo dispatch.\n");
    char* buffer = malloc(14);
    int recv_bytes = recv(kernelDispatchFd, buffer, 13, 0);
    buffer[recv_bytes] = '\0';
    printf("Mensaje: %s\n", buffer);
    free(buffer);

    t_PCB* pcb = malloc(sizeof(t_PCB));
    
    fetch_instruction(pcb);

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

    // Test thread & interrupt message from kernel. -- Borrar
    printf("Estoy dentro del hilo interrupt.\n");
    char* buffer = malloc(15);
    int recv_bytes = recv(kernelInterruptFd, buffer, 14, 0);
    buffer[recv_bytes] = '\0';
    printf("Mensaje: %s\n", buffer);
    free(buffer);

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

    // Thread en que escuchará los mensajes de dispatch enviados por KERNEL
    pthread_t THREAD_DISPATCH;
    if (!pthread_create(&THREAD_DISPATCH, NULL, (void*) dispatch_server, NULL))
        pthread_detach(THREAD_DISPATCH);
    else {
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

    for(;;);

    pthread_join(THREAD_DISPATCH, NULL);
    pthread_join(THREAD_INTERRUPT, NULL);

    cerrar_programa();
}
