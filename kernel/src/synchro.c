#include "synchro.h"

// Listas

// -- NEW
pthread_mutex_t MUTEX_LISTA_NEW;
sem_t CONTADOR_LISTA_NEW;

// General
pthread_mutex_t MUTEX_CURRENT_PID;

void initializeSemaphores(){
    pthread_mutex_init(&MUTEX_LISTA_NEW, NULL);
    sem_init(&CONTADOR_LISTA_NEW, 0, 0);
    pthread_mutex_init(&MUTEX_CURRENT_PID, NULL);
}