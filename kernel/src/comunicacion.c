#include "comunicacion.h"

// Tamanio por cada instruccion
// Incluye el entero previo que marca su size
uint32_t TAMANO_INSTRUCCION = sizeof(uint32_t) * 3;

typedef struct {
    int fd;
    char* server_name;
    t_log* logger;
} t_procesar_conexion_args;

static t_instruccion* recibir_instruccion (void* stream, uint16_t posicion){
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    // Ignoramos instrucciones previas
    stream += TAMANO_INSTRUCCION * posicion;

    //Agrego esto para skipear el tamanio de la instruccion que ya conocemos(8)    
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->codigo_instruccion), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(instruccion->parametro), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return instruccion;
}

static uint8_t recibir_header(t_paquete* paquete, int cliente_socket, t_log* logger){
    paquete->buffer = malloc(sizeof(t_buffer));

    if(recv(cliente_socket, &(paquete->codigo_operacion), sizeof(op_code), 0) == 0) {
        log_info(logger,"DISCONNECTED!!!");
        return 0;
    }
    if(paquete->codigo_operacion != ENVIAR_PSEUDO_CODIGO){
        log_info(logger,"El kernel server solo admite instrucciones");
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
    t_log* logger = args->logger;
    char* server_name = args->server_name;
    free(args);

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cliente_socket, logger) == 0){
        return;
    }
    for(uint16_t pos = 0; pos < (paquete->buffer->size / TAMANO_INSTRUCCION); pos++){
        t_instruccion* instruccion = recibir_instruccion(paquete->buffer->stream, pos);
        
        log_info(logger,"ACA !!!\nSize: %d\nTipo: %d\nArgumento: %d\n", 8, instruccion->codigo_instruccion, instruccion->parametro);
        free(instruccion);
    }

    eliminar_paquete(paquete);
    liberar_conexion(&cliente_socket);
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