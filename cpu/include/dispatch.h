#ifndef DISPATCH_H
#define DISPATCH_H

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
#include "cpu.h"

// Shared
#include "protocolo.h"
#include "shared_utils.h"


int exec_no_op ();
int exec_i_o (t_PCB*, uint32_t);
int exec_read (t_PCB*, t_instruc*);
int exec_write (t_PCB*, t_instruc*);
int exec_copy (t_PCB*, t_instruc*);
int exec_exit (t_PCB*);

#endif