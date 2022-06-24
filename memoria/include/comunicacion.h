#ifndef COMUNICACION_MEMORIA_H_
#define COMUNICACION_MEMORIA_H_

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>

#include "shared_utils.h"
#include "sockets.h"
#include "protocolo.h"
#include "crear_proceso.h"
#include "global.h"

void handler_kernel();

#endif