#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>


typedef enum {
	ENVIAR_PSEUDO_CODIGO,
	DEBUG_OP_CODE
} op_code;

typedef struct {
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef enum {
	NO_OP,
	I_O,
	READ,
	COPY,
	WRITE,
	EXIT
} instruccion_cod;

typedef struct {
	instruccion_cod codigo_instruccion;
	uint32_t param_1;
	uint32_t param_2;
} t_instruccion;

typedef struct {
    uint32_t    pid;                        // Process ID - Identificador del proceso
    uint32_t    process_size;               // Tamaño en bytes del proceso
    t_list*  	instructions_list;  		// Lista de instrucciones a ejecutar
    uint32_t    pc;                         // Program Counter - Número de la próxima instrucción a ejecutar
    uint32_t    page_table_id;              // Identificador de la tabla de páginas del proceso en memoria
    uint32_t    burst_prediction;           // Estimación para planificación bajo algoritmo SRT
} t_PCB;


t_paquete* crear_paquete(op_code codigo_operacion);
void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
bool send_codigo_op(int fd, op_code cop);

#endif