#ifndef TP_2022_1C_PASSIONE_INIT_H
#define TP_2022_1C_PASSIONE_INIT_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "config.h"

//Shared
#include "shared_utils.h"


uint8_t cargar_configuracion(t_config_consola*, t_log*);
void cerrar_programa(t_config_consola*, t_log*);
#endif