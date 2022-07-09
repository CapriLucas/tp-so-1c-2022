#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdio.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include <pthread.h>

#include "global.h"
#include "config.h"
#include "init.h"
#include "dispatch.h"
#include "interrupt.h"
#include "cpu.h"

// Shared
#include "protocolo.h"
#include "shared_utils.h"


// Execute instruction NO OP
int exec_no_op ();
// Execute instruction I/O
int exec_i_o (t_PCB*, uint32_t);
// Execute instruction READ
int exec_read (t_PCB*, t_instruccion*);
// Execute instruction WRITE
int exec_write (t_PCB*, t_instruccion*);
// Execute instrucction COPY
int exec_copy (t_PCB*, t_instruccion*);
// Execute instruction EXIT
int exec_exit (t_PCB*);
// Execute INTERRUPT
int return_pcb_interrupt (t_PCB* pcb);

// Fetch instruction
t_instruccion* fetch_instruction (t_PCB* pcb);
// Fetch operands
// int fetch_operands (t_PCB* pcb, t_instruccion* instruc);     -- Aún no implementada
// Execute instruction 
bool exec_instruction (t_PCB* pcb, t_instruccion* instruc);


// Dispatch server
void dispatch_server();


#endif