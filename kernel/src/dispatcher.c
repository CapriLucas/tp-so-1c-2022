#include "dispatcher.h"


t_PCB* pcb_srt;
static void* _rafaga_mas_corta(t_PCB* proceso1, t_PCB* proceso2) {
    return proceso1->burst_prediction <= proceso2->burst_prediction ? proceso1 : proceso2;
}

bool _pcb_a_sacar(t_PCB *some_pcb) {
    bool res = pcb_srt->pid == some_pcb->pid;
    return res;
}
t_PCB* get_next_pcb_to_exec(){
    if(strcasecmp(mainConfig->ALGORITMO_PLANIFICACION, "SRT") == 0){
        pcb_srt = list_get_minimum(LISTA_READY, (void*)_rafaga_mas_corta);
        list_remove_by_condition(LISTA_READY,(void*) _pcb_a_sacar);
        return pcb_srt;
    }
    t_PCB* pcb = list_get(LISTA_READY, 0);
    list_remove(LISTA_READY, 0);
    return pcb;
}