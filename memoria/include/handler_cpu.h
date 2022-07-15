#ifndef HANDLER_CPU_H
#define HANDLER_CPU_H

#include "global.h"
#include "protocolo.h"
#include "sockets.h"
#include "init.h"

// Handler CPU
void handler_cpu();

// Handshake CPU <-> MEMORY
int recv_msg_cpu_handshake();
int send_msg_cpu_handshake(uint32_t* page_size, uint32_t* input_table_qty);

// READ messages CPU <-> MEMORY
int recv_msg_cpu_read(uint32_t* physical_address);
int send_msg_cpu_read(uint32_t* value);

// WRITE messages CPU <-> MEMORY
int recv_msg_cpu_write(uint32_t* physical_address, uint32_t* value);
int send_msg_cpu_write();

// ACCESS MEMORY messages CPU <-> MEMORY
int recv_msg_cpu_access_memory_1st(uint32_t* page_number);
int send_msg_cpu_access_memory_1st(uint32_t* page_number);
int recv_msg_cpu_access_memory_1st(uint32_t* page_number);
int send_msg_cpu_access_memory_1st(uint32_t* frame_number);

#endif