#ifndef MGMT_ST_H
#define MGMT_ST_H

#include <stdint.h>
/* 
enum instruction_id {
    NO_OP,
    IO,
    READ,
    WRITE,
    COPY,
    EXIT
};


typedef struct {
    char*       id;    
    uint32_t    param_1;
    uint32_t    param_2;
} t_instruction;


typedef struct {
    t_instruction*  instruction;    
} t_instruction_set;


typedef struct {
    uint32_t    pid;                        // Process ID - Identificador del proceso
    uint32_t    process_size;               // Tamaño en bytes del proceso
    t_instruction_set*  t_instruction_set;  // Lista de instrucciones a ejecutar
    uint32_t    pc;                         // Program Couner - Número de la próxima instrucción a ejecutar
    uint32_t    page_table_id;              // Identificador de la tabla de páginas del proceso en memoria
    uint32_t    burst_prediction;           // Estimación para planificación bajo algoritmo SRT
} t_PCB;


 */
#endif