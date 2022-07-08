#ifndef MEDIANO_PLAZO_H
#define MEDIANO_PLAZO_H

#include "global.h"
#include <semaphore.h>
#include <pthread.h>

void handler_mediano_plazo();
void handler_check_blocked_timer(t_PCB_BLOCKED* pcb_to_suspend);

#endif
