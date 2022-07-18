#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/log.h>
#include <stdbool.h>
#include <commons/config.h>
#include <time.h>
#include <inttypes.h>


#include "protocolo.h"

bool config_has_all_properties(t_config* , char**);
char* leer_archivo_completo(char* path);
char* get_instruction_name(instruccion_cod cod);
instruccion_cod get_instruction_cod(char* string);

// Get timestamp in microseconds
uint64_t get_ms_timestamp();
// Set timestamp
int set_timestamp(struct timespec* tms);
// Compare timestamp and return true if a > b
bool timestamp_cmp(struct timespec* a, struct timespec* b);

#endif
