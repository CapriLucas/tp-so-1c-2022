#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct {

    uint32_t    ENTRADAS_TLB;               // Cantidad de entradas de la TLB
    char*       REEMPLAZO_TLB;              // Algoritmo de reemplazo para las entradas de la TLB: FIFO o LRU
    uint32_t    RETARDO_NOOP;               // Tiempo en milisegundos que se deberá esperar al ejecutar una instrucción NoOp
    char*       IP_MEMORIA;                 // IP a la cual se deberá conectar con la Memoria
    uint16_t    PUERTO_MEMORIA;             // Puerto al cual se deberá conectar con la Memoria
    uint16_t    PUERTO_ESCUCHA_DISPATCH;    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    uint16_t    PUERTO_ESCUCHA_INTERRUPT;   // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones

} t_config_CPU;


/*
    Ejemplo de archivo de configuración

        ENTRADAS_TLB=4
        REEMPLAZO_TLB=LRU
        RETARDO_NOOP=1000
        IP_MEMORIA=127.0.0.1
        PUERTO_MEMORIA=8002
        PUERTO_ESCUCHA_DISPATCH=8001
        PUERTO_ESCUCHA_INTERRUPT=8005
*/

typedef struct {
    uint32_t        page;   
    uint32_t        frame;
    struct timespec tms;
} t_TLB;

#endif