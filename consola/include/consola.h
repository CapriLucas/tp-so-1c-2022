#ifndef CONSOLA_H
#define CONSOLA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <commons/log.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <dirent.h>
#include <errno.h>


typedef struct {
    char* PATH;
    uint32_t MEMORY_SIZE;
} t_console_cmd;

#endif