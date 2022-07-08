#ifndef CORTO_PLAZO_H
#define CORTO_PLAZO_H


#include "global.h"
#include "sockets.h"
#include "protocolo.h"
#include "dispatcher.h"
#include "mediano_plazo.h"
#include <semaphore.h>
#include <pthread.h>

void handler_corto_plazo();
void handler_ciclo_corto_plazo();
void handler_check_ready_list();

#endif