#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <commons/log.h>
#include <stdbool.h>
#include <commons/config.h>

char* mi_funcion_compartida();
bool config_has_all_properties(t_config* , char**);
char* leer_archivo_completo(char* path);

#endif