#include "synchro.h"

// Listas

// -- NEW
pthread_mutex_t MUTEX_LISTA_NEW;
sem_t CONTADOR_LISTA_NEW;

// -- READY
pthread_mutex_t MUTEX_LISTA_READY;
sem_t CONTADOR_LISTA_READY;

// -- EXIT
pthread_mutex_t MUTEX_LISTA_EXIT;
sem_t CONTADOR_LISTA_EXIT;
pthread_mutex_t MUTEX_LISTA_EXIT_PID;

// General
pthread_mutex_t MUTEX_NEXT_PID;
sem_t GRADO_MULTIPROGRAMACION;

void initializeSemaphores(){
    pthread_mutex_init(&MUTEX_LISTA_NEW, NULL);
    sem_init(&CONTADOR_LISTA_NEW, 0, 0);

    pthread_mutex_init(&MUTEX_LISTA_READY, NULL);
    sem_init(&CONTADOR_LISTA_READY, 0, 0);

    pthread_mutex_init(&MUTEX_LISTA_EXIT, NULL);
    sem_init(&CONTADOR_LISTA_EXIT, 0, 0);
    pthread_mutex_init(&MUTEX_LISTA_EXIT_PID, NULL);

    pthread_mutex_init(&MUTEX_NEXT_PID, NULL);
    sem_init(&GRADO_MULTIPROGRAMACION, 0, mainConfig->GRADO_MULTIPROGRAMACION);
    
}