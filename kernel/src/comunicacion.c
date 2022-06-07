#include "comunicacion.h"

// Tamanio por cada instruccion
// Incluye el entero previo que marca su size
uint32_t TAMANO_INSTRUCCION = sizeof(uint32_t) * 4;

typedef struct {
    int  fd;
} t_procesar_conexion_args;


const char* get_instruction_name(instruccion_cod instruccion_cod) {

	switch(instruccion_cod) {
		case NO_OP:		return "NO_OP";
		case I_O: 		return "I/O";
		case READ:		return "READ";
		case COPY:		return "COPY";
		case WRITE:		return "WRITE";
		case EXIT:		return "EXIT";
		default:		return "";
	}
}

static t_instruccion* recibir_instruccion (void* stream, uint16_t posicion){
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    // Ignoramos instrucciones previas
    stream += TAMANO_INSTRUCCION * posicion;

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

static uint8_t recibir_header(t_paquete* paquete, int cliente_socket){
    paquete->buffer = malloc(sizeof(t_buffer));

    if(recv(cliente_socket, &(paquete->codigo_operacion), sizeof(op_code), 0) == 0) {
        log_info(mainLog, "DISCONNECTED!!!");
        return 0;
    }
    if(paquete->codigo_operacion != ENVIAR_PSEUDO_CODIGO){
        log_info(mainLog, "El kernel server solo admite instrucciones");
        return 0;
    }

    recv(cliente_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(cliente_socket, paquete->buffer->stream, paquete->buffer->size, 0);

    return 1;
}

static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    free(args);

    t_list* lista_instrucciones = list_create();

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cliente_socket) == 0){
        return;
    }

    for(uint16_t pos = 0; pos < (paquete->buffer->size / TAMANO_INSTRUCCION); pos++){
        t_instruccion* instruccion = recibir_instruccion(paquete->buffer->stream, pos);       
        list_add(lista_instrucciones, instruccion);

        //TODO esto se tiene que sacar al crear hilo de exit
        if (instruccion->codigo_instruccion == 5) {
            send(cliente_socket, "EXIT", 4, 0);
            liberar_conexion(&cliente_socket);
        }
    }

    //TODO ver campos hardcodeados al crear pcb de instruccion
    t_PCB* pcb = crear_pcb(8, lista_instrucciones, 0, 0, 0);
    
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, (void*) pcb);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    sem_post(&CONTADOR_LISTA_NEW);

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

    pthread_mutex_lock(&MUTEX_CURRENT_PID);
    pcb->pid = current_pid + 1;
    current_pid = current_pid + 1;
    pthread_mutex_unlock(&MUTEX_CURRENT_PID);
}