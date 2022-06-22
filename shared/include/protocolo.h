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

//Shared
#include "shared_utils.h"


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

// Message headers KERNEL <-> CPU
typedef enum {
	MSG_INTERRUPT,
	MSG_INTERRUPT_ACK,
	MSG_EXEC,
	MSG_I_O,
	MSG_EXIT
} message;

typedef enum {
	NO_OP,
	I_O,
	READ,
	COPY,
	WRITE,
	EXIT
} instruc;

typedef struct {
	instruc instruc_cod;
	uint32_t param_1;
	uint32_t param_2;
} t_instruc;

typedef struct {
    uint32_t    pid;                        // Process ID - Identificador del proceso
    uint32_t    process_size;               // Tamaño en bytes del proceso
    uint32_t    pc;                         // Program Couner - Número de la próxima instrucción a ejecutar
    uint32_t    page_table_id;              // Identificador de la tabla de páginas del proceso en memoria
    uint32_t    burst_prediction;           // Estimación para planificación bajo algoritmo SRT
	t_list*     l_instruc;					// Lista de instrucciones a ejecutar
} t_PCB;

/* 
An identifier declared as an enumeration constant has type int
typedef struct {
	uint8_t		message;
	uint32_t	payload_size;
} t_header;
 */

t_paquete* crear_paquete(op_code codigo_operacion);
void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
bool send_codigo_op(int fd, op_code cop);

// HEADER
// Serializa header
void* serialize_header(uint8_t message, uint32_t payload_size);
// Deserializa header
void deserialize_header(void* stream, uint8_t* message, uint32_t* payload_size);

// INSTRUCTIONS
// Serializa estructura t_instruc
void* serialize_instruc(t_instruc* instruc);
// Deserializa estructura t_instruc
void deserialize_instruc(void* stream, t_instruc* instruc);

// PCB
// Serializa estructura t_PCB (y su respectiva estructura t_instruc)
void* serialize_pcb(t_PCB* pcb);
// Deserializa estructura t_PCB (y su respectiva estructura t_instruc)
void deserialize_pcb(void* stream, t_PCB* pcb, size_t size);

// EXEC MSG
// Serializa mensaje EXEC (Kernel -> CPU)
void* serialize_msg_exec(t_PCB* pcb);
// Deserializa mensaje EXEC (Kernel -> CPU)
void deserialize_msg_exec(void* stream, uint32_t payload_size, t_PCB* pcb);

// I/O MSG
// Serializa mensaje I/O (CPU -> Kernel)
void* serialize_msg_i_o(uint32_t msec, t_PCB* pcb); 
// Deserializa mensaje I/O (CPU -> Kernel)
void deserialize_msg_i_o(void* stream, uint32_t payload_size, uint32_t* msec, t_PCB* pcb);

// EXIT MSG
// Serializa mensaje EXIT (CPU -> Kernel)
void* serialize_msg_exit(t_PCB* pcb);
// Deserializa mensaje EXIT (CPU -> Kernel)
void deserialize_msg_exit(void* stream, uint32_t payload_size, t_PCB* pcb);


#endif