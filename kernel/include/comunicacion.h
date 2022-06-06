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

#endif