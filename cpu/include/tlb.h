#ifndef TLB_H
#define TLB_H

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

// Leer entrada de página de TLB y retornar número de frame
int32_t get_frame(uint32_t page);
// Buscar índice en TLB para reemplazar
int get_index_to_replace();
// Buscar índice en TLB para reemplazar según algoritmo FIFO
int get_index_to_replace_fifo();
// Buscar índice en TLB para reemplazar según algoritmo LRU
int get_index_to_replace_lru();
// Escribir entrada de página de TLB
int set_frame(uint32_t page, uint32_t frame);

#endif