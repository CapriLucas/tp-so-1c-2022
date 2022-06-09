#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include <pthread.h>

#include "config.h"
#include "init.h"
#include "dispatch.h"
#include "interrupt.h"

// Shared
#include "protocolo.h"
#include "shared_utils.h"

t_config_CPU* config_CPU;
t_log* log_CPU;

int memoriaFd;
int cpuDispatchFd;
int kernelDispatchFd;
int cpuInterruptFd;
int kernelInterruptFd;

#endif
