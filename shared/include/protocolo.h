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
	MSG_CPU_MEM__HANDSHAKE,		// HANDSHAKE message from CPU to MEMORY :: ()
	MSG_MEM_CPU__HANDSHAKE,		// HANDSHAKE response from MEMORY to CPU :: (Tamaño de página, Cantidad de entradas por tabla)
	MSG_CPU_MEM__READ,			// READ message from CPU to MEMORY :: (Dirección física)
	MSG_MEM_CPU__READ,			// READ response from MEMORY to CPU :: 
	MSG_CPU_MEM__WRITE,			// WRITE message from CPU to MEMORY :: 
	MSG_MEM_CPU__WRITE,			// WRITE response from MEMORY to CPU ::
	MSG_CPU_MEM__PAGE_ACCESS_1ST,	// PAGE TABLE ACCESS 1st LEVEL message from CPU to MEMORY :: 
	MSG_CPU_MEM__PAGE_ACCESS_2ND,	// PAGE TABLE ACCESS 2nd LEVEL message from CPU to MEMORY :: 
	MSG_MEM_CPU__PAGE_ACCESS_1ST,	// PAGE TABLE ACCESS 1st LEVEL response from MEMORY to CPU :: 
	MSG_MEM_CPU__PAGE_ACCESS_2ND	// PAGE TABLE ACCESS 2nd LEVEL response from MEMORY to CPU :: 
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
void destroy_pcb(t_PCB* pcb);


/* ################################ 
		   CPU <-> KERNEL
################################ */

// EXEC MSG (Kernel -> CPU)
// Serializa mensaje EXEC
t_paquete* serialize_msg_exec(t_PCB* pcb);
// Deserializa mensaje EXEC
void deserialize_msg_exec(t_paquete* paquete, t_PCB* pcb);

// I/O MSG (CPU -> Kernel)
// Serializa mensaje I/O
t_paquete* serialize_msg_i_o(uint32_t msec, t_PCB* pcb); 
// Deserializa mensaje I/O
void deserialize_msg_i_o(t_paquete* paquete, uint32_t* msec, t_PCB* pcb);

// EXIT MSG (CPU -> Kernel)
// Serializa mensaje EXIT 
t_paquete* serialize_msg_exit(t_PCB* pcb);
// Deserializa mensaje EXIT
void deserialize_msg_exit(t_paquete* paquete, t_PCB* pcb);

// INTERRUPT MSG (Kernel -> CPU)
// Serializa mensaje INTERRUPT
t_paquete* serialize_msg_interrupt();

// INTERRUPT ACKNOWLEDGMENT MSG (CPU -> Kernel)
// Serializa mensaje INTERRUPT ACK
t_paquete* serialize_msg_interrupt_ack(t_PCB* pcb);
// Deseriliza mensaje INTERRUPT ACK
void deserialize_msg_interrupt_ack(t_paquete* paquete, t_PCB* pcb);


/* ################################ 
		   CPU <-> MEMORY
################################ */

/*
 *		HANDSHAKE MSG
 */

// CPU -> MEMORY

// Serializa mensaje HANDSHAKE
t_paquete* serialize_msg_cpu_mem_handshake();

// MEMORY -> CPU

// Serializa mensaje HANDSHAKE ACK
t_paquete* serialize_msg_mem_cpu_handshake(uint32_t* page_size, uint32_t* input_table_qty);
// Deserializa mensaje HANDSHAKE ACK
void deserialize_msg_mem_cpu_handshake(t_paquete* paquete, uint32_t* page_size, uint32_t* input_table_qty);


/*
 *		READ MSG
 */

// CPU -> MEMORY

// Serializa mensaje READ
t_paquete* serialize_msg_cpu_mem_read(uint32_t* physical_address);
// Deserializa mensaje READ
void deserialize_msg_cpu_mem_read(t_paquete* paquete, uint32_t* physical_address);

// MEMORY -> CPU

// Serializa mensaje READ RESPONSE
t_paquete* serialize_msg_mem_cpu_read(uint32_t* value);
// Deserializa mensaje READ RESPONSE
void deserialize_msg_mem_cpu_read(t_paquete* paquete, uint32_t* value);


/*
 *		WRITE MSG
 */

// CPU -> MEMORY

// Serializa mensaje WRITE
t_paquete* serialize_msg_cpu_mem_write(uint32_t* physical_address, uint32_t* value);
// Deserializa mensaje WRITE
void deserialize_msg_cpu_mem_write(t_paquete* paquete, uint32_t* physical_address, uint32_t* value);

// MEMORY -> CPU

// Serializa mensaje WRITE RESPONSE
t_paquete* serialize_msg_mem_cpu_write();


/*
 *		MEMORY ACCESS MSG
 */

// CPU -> MEMORY
// Serializa mensaje PAGE TABLE ACCESS 1st LEVEL
t_paquete* serialize_msg_cpu_mem_page_access_1st(uint32_t* page_number);
// Deserializa mensaje PAGE TABLE ACCESS 1st LEVEL
void deserialize_msg_cpu_mem_page_access_1st(t_paquete* paquete, uint32_t* page_number);
// Serializa mensaje PAGE TABLE ACCESS 2nd LEVEL
t_paquete* serialize_msg_cpu_mem_page_access_2nd(uint32_t* page_number);
// Deserializa mensaje PAGE TABLE ACCESS 2nd LEVEL
void deserialize_msg_cpu_mem_page_access_2nd(t_paquete* paquete, uint32_t* page_number);

// MEMORY -> CPU
// PAGE TABLE ACCESS 1st LEVEL RESPONSE
// Serializa mensaje PAGE TABLE ACCESS 1st LEVEL RESPONSE
t_paquete* serialize_msg_mem_cpu_page_access_1st(uint32_t* page_number);
// Deserializa mensaje PAGE TABLE ACCESS 1st LEVEL RESPONSE
void deserialize_msg_mem_cpu_page_access_1st(t_paquete* paquete, uint32_t* page_number);
// PAGE TABLE ACCESS 2nd LEVEL RESPONSE
// Serializa mensaje PAGE TABLE ACCESS 2nd LEVEL RESPONSE
t_paquete* serialize_msg_mem_cpu_page_access_2nd(uint32_t* frame_number);
// Deserializa mensaje PAGE TABLE ACCESS 2nd LEVEL RESPONSE
void deserialize_msg_mem_cpu_page_access_2nd(t_paquete* paquete, uint32_t* frame_number);

/* ################################ 
################################ */


#endif