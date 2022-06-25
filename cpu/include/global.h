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
extern t_config_CPU* config_CPU;
extern t_log* log_CPU;

// File descriptors
extern int memoriaFd;
extern int cpuDispatchFd;
extern int kernelDispatchFd;
extern int cpuInterruptFd;
extern int kernelInterruptFd;

// NEXT_PID
extern bool interrupt;
extern pthread_mutex_t MUTEX_INTERRUPT;

#endif