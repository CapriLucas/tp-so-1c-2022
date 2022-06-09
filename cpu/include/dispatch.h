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


int exec_no_op (t_PCB*);
int exec_i_o (t_PCB*);
int exec_read (t_PCB*);
int exec_write (t_PCB*);
int exec_copy (t_PCB*);
int exec_exit (t_PCB*);

#endif