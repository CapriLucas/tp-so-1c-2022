#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h> 
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdbool.h>
#include <semaphore.h>

#include "config.h"
#include "protocolo.h"

//TODO mover de aca
typedef struct {
    int console_fd;
    uint32_t pid;    
} t_socket_pid;

typedef struct {
    t_PCB* pcb;
    uint32_t msec;    
} t_PCB_BLOCKED;

#define SERVERNAME "KERNEL_SERVER"

//Global variables
extern t_log* mainLog;
extern t_config_kernel* mainConfig;

// File descriptors
extern int server_fd;
extern int memoriaFd;
extern int kernelDispatchFd;
extern int kernelInterruptFd;

// NEXT_PID
extern uint32_t next_pid;
extern pthread_mutex_t MUTEX_NEXT_PID;

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
extern sem_t LISTA_READY_INTERRUPT;

// -- Lista Exit
extern t_list* LISTA_EXIT;
extern t_list* LISTA_EXIT_PID;
extern pthread_mutex_t MUTEX_LISTA_EXIT;
extern sem_t CONTADOR_LISTA_EXIT;
extern pthread_mutex_t MUTEX_LISTA_EXIT_PID;

// -- Lista BLOCKED
extern t_list* LISTA_BLOCKED;
extern pthread_mutex_t MUTEX_LISTA_BLOCKED;
extern sem_t CONTADOR_LISTA_BLOCKED;

// -- Lista SUSPENDED_BLOCKED
extern t_list* LISTA_SUSPENDED_BLOCKED;
extern pthread_mutex_t MUTEX_LISTA_SUSPENDED_BLOCKED;
extern sem_t CONTADOR_LISTA_SUSPENDED_BLOCKED;

// -- Lista SUSPENDED_READY
extern t_list* LISTA_SUSPENDED_READY;
extern pthread_mutex_t MUTEX_LISTA_SUSPENDED_READY;
extern sem_t CONTADOR_LISTA_SUSPENDED_READY;

#endif