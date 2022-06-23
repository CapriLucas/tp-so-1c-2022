#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdio.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

#include "config.h"
#include "init.h"

// Shared
#include "protocolo.h"
#include "shared_utils.h"


t_log* log_Memoria;
t_config_Memoria* config_Memoria;

int memoriaFd;
int kernelFd;
int cpuFd;


#endif