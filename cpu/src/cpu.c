#include "cpu.h"

t_log* mainLog;
t_config_CPU* mainConfig;


static void inicializar_proceso() {
    mainLog = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_CPU));
    if (!cargar_configuracion(mainConfig, mainLog)) {
        exit(EXIT_FAILURE);
    }
}

void dispatch_server() {
    
    int cpuDispatchFd;
    int kernelDispatchFd;

    // Inicia server en puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    cpuDispatchFd = iniciar_servidor (
        mainLog,
        "CPU",
        "127.0.0.1", 
        "8001"  // mainConfig->PUERTO_ESCUCHA_DISPATCH
    );

    if (!cpuDispatchFd) {
        liberar_conexion(&cpuDispatchFd);
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de KERNEL para mensajes de dispatch
    kernelDispatchFd = esperar_cliente (
        mainLog, 
        "KERNEL", 
        cpuDispatchFd
    );

    // Test thread & dispatch message from kernel. -- Borrar
    printf("Estoy dentro del hilo dispatch.\n");
    char* buffer = malloc(14);
    int recv_bytes = recv(kernelDispatchFd, buffer, 13, 0);
    buffer[recv_bytes] = '\0';
    printf("Mensaje: %s\n", buffer);
    free(buffer);

    t_PCB* pcb;
    fetch_instruction(pcb, mainConfig, mainLog);

    liberar_conexion(&cpuDispatchFd);
    liberar_conexion(&kernelDispatchFd);
}


void interrupt_server() {

    int cpuInterruptFd;
    int kernelInterruptFd;

    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    cpuInterruptFd = iniciar_servidor (
        mainLog,
        "CPU",
        "127.0.0.1", 
        "8005"  // mainConfig->PUERTO_ESCUCHA_INTERRUPT
    );

    if (!cpuInterruptFd) {    
        liberar_conexion(&cpuInterruptFd);
        exit(EXIT_FAILURE);
    }

    // Esperar conexión de KERNEL para mensajes de interrupciones
    kernelInterruptFd = esperar_cliente (
        mainLog, 
        "KERNEL", 
        cpuInterruptFd
    );

    // Test thread & interrupt message from kernel. -- Borrar
    printf("Estoy dentro del hilo interrupt.\n");
    char* buffer = malloc(15);
    int recv_bytes = recv(kernelInterruptFd, buffer, 14, 0);
    buffer[recv_bytes] = '\0';
    printf("Mensaje: %s\n", buffer);
    free(buffer);



    liberar_conexion(&cpuInterruptFd);
    liberar_conexion(&kernelInterruptFd);
}


int fetch_instruction (t_PCB* pcb, t_config_CPU* config, t_log* log) {
    
    t_instruccion* instruction = malloc(sizeof(t_instruccion));

    int instruction_code = 0; 
    instruction->param_1 = 3;
    //instruction = pcb->t_instruccion[pcb->pc];

    //instruction_code* instruction_code = instruccion->instruccion_cod

    switch (instruction_code) {
        
        case NO_OP:
            exec_no_op(instruction, config, log);
            break;
        case I_O:
            exec_i_o(instruction, config, log);
            break;
        case READ:
            exec_read(instruction, config, log);
            break;
        case WRITE:
            exec_write(instruction, config, log);
            break;
        case COPY:
            exec_copy(instruction, config, log);
            break;
        case EXIT:
            exec_exit(instruction, config, log);
            break;

    }

    return EXIT_SUCCESS;
}


int main(){

    inicializar_proceso();

    // Test load cpu.config -- Borrar
    printf("ENTRADAS_TLB: %u\n", mainConfig->ENTRADAS_TLB);
    printf("REEMPLAZO_TLB: %s\n", mainConfig->REEMPLAZO_TLB);
    printf("RETARDO_NOOP: %u\n", mainConfig->RETARDO_NOOP);
    printf("IP_MEMORIA: %s\n", mainConfig->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %u\n", mainConfig->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA_DISPATCH: %u\n", mainConfig->PUERTO_ESCUCHA_DISPATCH);
    printf("PUERTO_ESCUCHA_INTERRUPT: %u\n", mainConfig->PUERTO_ESCUCHA_INTERRUPT);

    
    // Crear conexión con MEMORIA
    int memoriaFd;
    memoriaFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

    if (!memoriaFd) {        
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    // Thread en que escuchará los mensajes de dispatch enviados por KERNEL
    pthread_t THREAD_DISPATCH;
    if (!pthread_create(&THREAD_DISPATCH, NULL, (void*) dispatch_server, NULL))
        pthread_detach(THREAD_DISPATCH);
    else {
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    // Thread en que escuchará los mensajes de INTERRUPT enviados por KERNEL
    pthread_t THREAD_INTERRUPT;
    if (!pthread_create(&THREAD_INTERRUPT, NULL, (void*) interrupt_server, NULL))
        pthread_detach(THREAD_INTERRUPT);
    else {
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }  

    for(;;);
    pthread_join(THREAD_DISPATCH, NULL);
    pthread_join(THREAD_INTERRUPT, NULL);

    cerrar_programa(mainConfig, mainLog, &memoriaFd);
}
