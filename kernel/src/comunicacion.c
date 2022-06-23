#include "comunicacion.h"

// Tamanio por cada instruccion
// Incluye el entero previo que marca su size
uint32_t TAMANO_INSTRUCCION = sizeof(uint32_t) * 4;

typedef struct {
    int  fd;
    char* server_name;
    t_log* logger;
} t_procesar_conexion_args;


const char* get_instruction_name(instruccion_cod instruc_cod) {

	switch(instruc_cod) {
		case NO_OP:		return "NO_OP";
		case I_O: 		return "I/O";
		case READ:		return "READ";
		case COPY:		return "COPY";
		case WRITE:		return "WRITE";
		case EXIT:		return "EXIT";
		default:		return "";
	}
}

void procesar_instruccion(t_instruccion* instruccion, int client_fd, t_log* logger) {

    log_info(logger,"Size: %d\nCódigo instrucción: %s\nParámetro 1: %d\nParámetro 2: %d\n", 
        8,      // Memory size
        get_instruction_name(instruccion->codigo_instruccion), 
        instruccion->param_1, 
        instruccion->param_2
    );
        
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
/*     memcpy(&(instruccion->parametro), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t); */

    memcpy(&(instruccion->param_1), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->param_2), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return instruccion;
}

static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    t_log* logger = args->logger;
    char* server_name = args->server_name;
    free(args);

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cliente_socket) == 0){
        return;
    }
    if(paquete->codigo_operacion != ENVIAR_PSEUDO_CODIGO){
        log_info(logger,"El kernel server solo admite instrucciones");
        return;
    }

    for(uint16_t pos = 0; pos < (paquete->buffer->size / TAMANO_INSTRUCCION); pos++){
        t_instruccion* instruccion = recibir_instruccion(paquete->buffer->stream, pos);       

        procesar_instruccion(instruccion, cliente_socket, logger);

    }
    eliminar_paquete(paquete);
    // liberar_conexion(&cliente_socket);   Se debe mantener la conexión hasta tanto sea procesada la instrucción EXIT.
    return;
    
}

int server_escuchar(char* server_name, int server_fd, t_log* logger) {
    int cliente_socket = esperar_cliente(logger, server_name, server_fd);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->fd = cliente_socket;
        args->logger = logger;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}
