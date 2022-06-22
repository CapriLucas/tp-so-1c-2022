#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

#include "config.h"

//Global variables
extern t_log* mainLog;
extern t_config_Memoria* mainConfig;

// File descriptors
extern int kernelFd;
extern int memoriaFd;
extern int cpuFd;

#endif