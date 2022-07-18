#include "tlb.h"

// Leer entrada de página de TLB y retornar número de frame
int32_t get_frame(uint32_t page) {
    
    uint32_t frame = -1;

    for (int i=0; i < config_CPU->ENTRADAS_TLB; i++) {
        if (tlb[i].page == page ) {
            frame = tlb[i].frame;

            // If LRU then update timestamp
            if (strcmp(config_CPU->REEMPLAZO_TLB, "LRU") == 0)
                set_timestamp(&(tlb[i].tms));
        
            break;
        }
    }

    return frame;
}

// Buscar índice en TLB para reemplazar
int get_index_to_replace() {

    int index = 0;
    struct timespec* ms = &(tlb[0].tms);

    for (int i = 1; i < config_CPU->ENTRADAS_TLB; i++) {
        if (timestamp_cmp(ms, &(tlb[i].tms))) {
            ms = &(tlb[i].tms);
            index = i;
        }
    }
    
    return index;
}

// Escribir entrada de página de TLB
int set_frame(uint32_t page, uint32_t frame) {

    int i = get_index_to_replace();

    tlb[i].page = page;
    tlb[i].frame = frame;
    set_timestamp(&(tlb[i].tms));

    return i;
}
