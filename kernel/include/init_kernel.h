#ifndef TP_2022_1C_PASSIONE_INIT_H
#define TP_2022_1C_PASSIONE_INIT_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "config.h"

//Global variables

#include "kernel.h"
#include "synchro.h"

//Shared
#include "sockets.h"
#include "shared_utils.h"


uint8_t cargar_configuracion(t_config_kernel*, t_log*);
uint8_t generar_conexiones(int*, t_config_kernel*, t_log*);
void cerrar_programa(t_config_kernel*, t_log*, int* server_fd);
bool crear_servidor(t_config_kernel* cfg, t_log* logger, int* fd, char* name);
void initializeKernel();

#endif