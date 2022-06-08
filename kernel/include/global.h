#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h> 
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdbool.h>
#include <semaphore.h>

#include "config.h"

#define SERVERNAME "KERNEL_SERVER"

//Global variables
extern t_log* mainLog;
extern t_config_kernel* mainConfig;

// File descriptors
extern int server_fd;
extern int memoriaFd;
extern int kernelDispatchFd;
extern int kernelInterruptFd;

// CURRENT_PID
extern uint32_t current_pid;
extern pthread_mutex_t MUTEX_CURRENT_PID;

// Nivel de multiprogramacion
extern sem_t GRADO_MULTIPROGRAMACION;

// Listas/colas de procesos

// -- Lista NEW
extern t_list* LISTA_NEW;
extern pthread_mutex_t MUTEX_LISTA_NEW;
extern sem_t CONTADOR_LISTA_NEW;

// -- Lista READY
extern t_list* LISTA_READY;
extern pthread_mutex_t MUTEX_LISTA_READY;
extern sem_t CONTADOR_LISTA_READY;

#endif