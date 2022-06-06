#ifndef KERNEL_H
#define KERNEL_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>

#include "config.h"
#include "comunicacion.h"
#include "init_kernel.h"
#include "shared_utils.h"

#define SERVERNAME "KERNEL_SERVER"

//Global variables

extern t_log* mainLog;
extern t_config_kernel* mainConfig;
extern int server_fd;

#endif