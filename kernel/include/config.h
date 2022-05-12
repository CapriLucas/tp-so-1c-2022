#ifndef TP_2022_1C_PASSIONE_CONFIG_H
#define TP_2022_1C_PASSIONE_CONFIG_H

#include <stdint.h>

typedef struct {
    // IPs y puertos
    uint16_t PUERTO_ESCUCHA;    
    char* IP_MEMORIA;
    char* IP_CPU;
    uint16_t PUERTO_MEMORIA;
    uint16_t PUERTO_CPU_DISPATCH;
    uint16_t PUERTO_CPU_INTERRUPT;

    // Planificacion
    char* ALGORITMO_PLANIFICACION;
    uint16_t ESTIMACION_INICIAL;    
    double ALFA;
    uint16_t GRADO_MULTIPROGRAMACION;    
    uint16_t TIEMPO_MAXIMO_BLOQUEADO;    
    
} t_config_kernel;

#endif
