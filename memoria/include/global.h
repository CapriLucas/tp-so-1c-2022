#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h> 
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdbool.h>
#include <semaphore.h>

#include "config.h"


// Global variables
extern t_config_Memoria* config_Memoria;
extern t_log* log_Memoria;

// File descriptors
extern int kernelFd;
extern int memoriaFd;
extern int cpuFd;

#endif