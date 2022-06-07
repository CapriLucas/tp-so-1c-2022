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

// Shared
#include "protocolo.h"
#include "shared_utils.h"


int exec_no_op (t_instruccion*, t_config_CPU*, t_log*);
int exec_i_o (t_instruccion*, t_config_CPU*, t_log*);
int exec_read (t_instruccion*, t_config_CPU*, t_log*);
int exec_write (t_instruccion*, t_config_CPU*, t_log*);
int exec_copy (t_instruccion*, t_config_CPU*, t_log*);
int exec_exit (t_instruccion*, t_config_CPU*, t_log*);

#endif