#ifndef INIT_H
#define INIT_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "global.h"
#include "config.h"
#include "memoria.h"

// Shared
#include "sockets.h"
#include "shared_utils.h"

void inicializar_proceso();
uint8_t cargar_configuracion(t_config_Memoria*, t_log*);
uint8_t generar_conexiones(int*, t_config_Memoria*, t_log*);
void cerrar_programa();

#endif