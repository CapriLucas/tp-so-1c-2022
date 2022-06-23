#include "comunicacion.h"

// Tamanio por cada instruccion
// Incluye el entero previo que marca su size
uint32_t TAMANO_INSTRUCCION = sizeof(uint32_t) * 4;

typedef struct {
    int  fd;
} t_procesar_conexion_args;


static uint32_t recibir_memory_size(void* stream) {
    uint32_t process_memory_size;

    //Salteamos el tamanio conocido
    stream += sizeof(uint32_t);
    //Copiamos el memory_size y lo retornamos
    memcpy(&(process_memory_size), stream, sizeof(uint32_t));
    return process_memory_size;
}

static t_instruccion* recibir_instruccion (void* stream, uint16_t posicion){
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    // Ignoramos instrucciones previas y el memory_size
    stream += (sizeof(uint32_t) * 2 + TAMANO_INSTRUCCION * posicion);

    //Agrego esto para skipear el tamanio de la instruccion que ya conocemos(8)    
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->codigo_instruccion), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->param_1), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->param_2), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return instruccion;
}

static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    free(args);

    uint32_t process_memory_size;
    t_list* lista_instrucciones = list_create();

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cliente_socket) == 0){
        return;
    }
    if(paquete->codigo_operacion != ENVIAR_PSEUDO_CODIGO){
        log_info(mainLog, "El kernel server solo admite instrucciones");
        return;
    }

    // Recibo primero el memory size del proceso y luego todas las instrucciones
    process_memory_size = recibir_memory_size(paquete->buffer->stream);

    // Le restamos el espacio del "memory_size"
    uint16_t cantInstrucciones = ((paquete->buffer->size - sizeof(uint32_t) * 2) / TAMANO_INSTRUCCION);

    for(uint16_t pos = 0; pos < cantInstrucciones ; pos++){
        t_instruccion* instruccion = recibir_instruccion(paquete->buffer->stream, pos);       
        list_add(lista_instrucciones, instruccion);

        //TODO sacar esto para testear comunicacion
        // printf("memory: %d\npos: %d\nOp: %s\nparam1: %d; param2: %d\n\n",process_memory_size, pos,get_instruction_name(instruccion->codigo_instruccion),instruccion->param_1,instruccion->param_2);
        
    }

    //TODO ver campos hardcodeados al crear pcb de instruccion
    t_PCB* pcb = crear_pcb(process_memory_size, lista_instrucciones, 0, 0, 0);
    
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, (void*) pcb);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    sem_post(&CONTADOR_LISTA_NEW);
    
    t_socket_pid* socket_pid = malloc(sizeof(t_socket_pid));
    
    socket_pid->pid = pcb->pid;
    socket_pid->console_fd = cliente_socket;
    pthread_mutex_lock(&MUTEX_LISTA_EXIT_PID);
    list_add(LISTA_EXIT_PID, (void*) socket_pid);
    pthread_mutex_unlock(&MUTEX_LISTA_EXIT_PID);

    eliminar_paquete(paquete);
    return;
    
}

int server_escuchar(char* server_name, int server_fd) {
    // Bloqueante para que el hilo no tenga espera activa
    int cliente_socket = esperar_cliente(mainLog, server_name, server_fd);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->fd = cliente_socket;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

t_PCB* crear_pcb(uint32_t process_size, 
                t_list* instructions_list, uint32_t pc, 
                uint32_t page_table_id, uint32_t burst_prediction){

    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb->process_size = process_size;
    pcb->instructions_list = instructions_list;
    pcb->pc = pc;
    pcb->page_table_id = page_table_id;
    pcb->burst_prediction = burst_prediction;

    pthread_mutex_lock(&MUTEX_NEXT_PID);
    pcb->pid = next_pid;
    next_pid = next_pid + 1;
    pthread_mutex_unlock(&MUTEX_NEXT_PID);

    return pcb;
}