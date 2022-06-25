#include "crear_proceso.h"

t_creacion_memoria* recibir_mensaje (void* stream);

void crear_proceso(t_paquete* paquete){

    t_creacion_memoria* datos_creacion = recibir_mensaje(paquete->buffer->stream);
    log_info(log_Memoria, "Pid: %d\n\nProcess_size:%d",datos_creacion->pid,datos_creacion->process_size);
    // TODO Implementar logica aca
    // Retornamos dato mockeado
    uint32_t page_table_id = 3;
    send(kernelFd, &page_table_id, sizeof(uint32_t), 0);
}

t_creacion_memoria* recibir_mensaje (void* stream){
    t_creacion_memoria* msg = malloc(sizeof(t_creacion_memoria));

    stream += sizeof(uint32_t);
    memcpy(&(msg->pid), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(msg->process_size), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return msg;
}