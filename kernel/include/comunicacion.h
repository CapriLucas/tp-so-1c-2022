#ifndef COMUNICACION_KERNEL_H_
#define COMUNICACION_KERNEL_H_

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>

#include "shared_utils.h"
#include "sockets.h"
#include "protocolo.h"
#include "global.h"


int server_escuchar(char* server_name, int server_socket);
t_PCB* crear_pcb(uint32_t process_size, t_list* instructions_list, uint32_t pc, uint32_t page_table_id, uint32_t burst_prediction);
#endif