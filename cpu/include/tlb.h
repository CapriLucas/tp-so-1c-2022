#ifndef TLB_H
#define TLB_H

#include <stdint.h>

typedef struct {
    uint32_t    page;   
    uint32_t    frame;
    // timestamp load   // Instante de carga
    // timestamp ref    // Instante de últimas referencia
} t_TLB;

// Buscar entrada de página


// Obtener entrada reemplazo algoritmo FIFO

// Obtener entrada reemplazo algoritmo LRU

#endif