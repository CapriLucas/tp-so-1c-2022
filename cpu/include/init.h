#ifndef INIT_H
#define INIT_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "config.h"
#include "cpu.h"

// Shared
#include "sockets.h"
#include "shared_utils.h"


uint8_t cargar_configuracion(t_config_CPU*, t_log*);
uint8_t generar_conexiones(int*, t_config_CPU*, t_log*);
void cerrar_programa();

#endif