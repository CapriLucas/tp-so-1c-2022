#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct {

    uint16_t    PUERTO_ESCUCHA;         // Puerto en el cual se escuchará la conexión de módulo
    uint32_t    TAM_MEMORIA;            // Tamaño expresado en bytes del tamaño de la memoria
    uint32_t    TAM_PAGINA;             // Tamaño de las páginas en bytes
    uint32_t    ENTRADAS_POR_TABLA;     // Cantidad de entradas de cada tabla de páginas
    uint32_t    RETARDO_MEMORIA;        // Tiempo en milisegundos que se deberá esperar para dar una respuesta al CPU
    char*       ALGORITMO_REEMPLAZO;    // Algoritmo de reemplazo de páginas (CLOCK/CLOCK-M)
    uint32_t    MARCOS_POR_PROCESO;     // Cantidad de marcos permitidos por proceso en asignación fija
    uint32_t    RETARDO_SWAP;           // Tiempo en milisegundos que se deberá esperar para cada operación del SWAP (leer/escribir)
    char*       PATH_SWAP;              // Carpeta donde se van a encontrar los archivos de SWAP de cada proceso

} t_config_Memoria;

/*
    Ejemplo de archivo de configuración:

        PUERTO_ESCUCHA=8002
        TAM_MEMORIA=4096
        TAM_PAGINA=64
        ENTRADAS_POR_TABLA=4
        RETARDO_MEMORIA=1000
        ALGORITMO_REEMPLAZO=CLOCK-M
        MARCOS_POR_PROCESO=4
        RETARDO_SWAP=2000
        PATH_SWAP=/home/utnso/swap
*/

#endif
