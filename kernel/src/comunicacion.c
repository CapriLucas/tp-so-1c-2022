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

// Por cada instruccion recibida la guardamos en la lista de NEW y sumamos uno a su contador
void procesar_instruccion(t_instruccion* instruccion, int client_fd) {

    log_info(mainLog,"Size: %d\nCódigo instrucción: %s\nParámetro 1: %d\nParámetro 2: %d\n", 
        8,      // Memory size
        get_instruction_name(instruccion->codigo_instruccion), 
        instruccion->param_1, 
        instruccion->param_2
    );
    
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, (void*) instruccion);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    sem_post(&CONTADOR_LISTA_NEW);

//TODO esto se tiene que sacar al crear hilo de exit
    if (instruccion->codigo_instruccion == 5) {
        send(client_fd, "EXIT", 4, 0);
        liberar_conexion(&client_fd);
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

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cliente_socket) == 0){
        return;
    }

    for(uint16_t pos = 0; pos < (paquete->buffer->size / TAMANO_INSTRUCCION); pos++){
        t_instruccion* instruccion = recibir_instruccion(paquete->buffer->stream, pos);       

        procesar_instruccion(instruccion, cliente_socket);
    }

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
