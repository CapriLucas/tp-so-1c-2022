#ifndef CONSOLA_H
#define CONSOLA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

#include "config.h"
#include "codigo.h"
#include "init_consola.h"

//Shared
#include "protocolo.h"
#include "shared_utils.h"
#include <dirent.h>
#include <errno.h>


typedef struct {
    char* PATH;
    uint32_t MEMORY_SIZE;
} t_console_cmd;

#endif