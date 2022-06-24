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
	CREAR_PROCESO_EN_MEMORIA,
	MSG_INTERRUPT,				// INTERRUPT message from KERNEL to CPU :: ()
	MSG_INTERRUPT_ACK,			// INTERRUPT ACKNOWLEDGEMENT message from CPU to KERNEL :: (PCB)
	MSG_EXEC,					// EXECUTE message from KERNEL to CPU :: (PCB)
	MSG_I_O,					// I/O message from CPU to KERNEL :: (PCB, I/O miliseconds)
	MSG_EXIT,					// EXIT message from CPU to KERNEL :: (PCB)
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
	NO_OP,			// NO OPERATION :: ()
	I_O,			// I/O :: (Miliseconds)
	READ,			// READ :: (Dirección lógica)
	COPY,			// COPY :: (Dirección lógica destino, Dirección lógica origen)
	WRITE,			// WRITE :: (Dirección lógica, Valor)
	EXIT			// EXIT :: ()
} instruccion_cod;

typedef struct {
	instruccion_cod codigo_instruccion;
	uint32_t param_1;
	uint32_t param_2;
} t_instruccion;

typedef struct {
    uint32_t    pid;                        // Process ID - Identificador del proceso
    uint32_t    process_size;               // Tamaño en bytes del proceso
    uint32_t    pc;                         // Program Couner - Número de la próxima instrucción a ejecutar
    uint32_t    page_table_id;              // Identificador de la tabla de páginas del proceso en memoria
    uint32_t    burst_prediction;           // Estimación para planificación bajo algoritmo SRT
	t_list*     instructions_list;			// Lista de instrucciones a ejecutar
} t_PCB;

typedef struct {
    uint32_t    pid;                        // Process ID - Identificador del proceso
    uint32_t    process_size;               // Tamaño en bytes del proceso
} t_creacion_memoria;

t_paquete* crear_paquete(op_code codigo_operacion);
void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
bool send_codigo_op(int fd, op_code cop);
uint8_t recibir_header(t_paquete* paquete, int fd);

// EXEC MSG
// Serializa mensaje EXEC (Kernel -> CPU)
t_paquete* serialize_msg_exec(t_PCB* pcb);
// Deserializa mensaje EXEC (Kernel -> CPU)
void deserialize_msg_exec(t_paquete* paquete, t_PCB* pcb);

// I/O MSG
// Serializa mensaje I/O (CPU -> Kernel)
t_paquete* serialize_msg_i_o(uint32_t msec, t_PCB* pcb); 
// Deserializa mensaje I/O (CPU -> Kernel)
void deserialize_msg_i_o(t_paquete* paquete, uint32_t* msec, t_PCB* pcb);

// EXIT MSG
// Serializa mensaje EXIT (CPU -> Kernel)
t_paquete* serialize_msg_exit(t_PCB* pcb);
// Deserializa mensaje EXIT (CPU -> Kernel)
void deserialize_msg_exit(t_paquete* paquete, t_PCB* pcb);


#endif