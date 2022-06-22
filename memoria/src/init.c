#include "init.h"

void inicializar_proceso() {

    // Memoria log
    log_Memoria = log_create("./logs/memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);

    // Memoria config
    config_Memoria = malloc(sizeof(t_config_Memoria));
    if (!cargar_configuracion(config_Memoria, log_Memoria)) {
        exit(EXIT_FAILURE);
    }
}


void cerrar_programa() {

    // Sockets
    if (memoriaFd) { liberar_conexion(&memoriaFd); }
    if (kernelFd) { liberar_conexion(&kernelFd); }
    if (cpuFd) { liberar_conexion(&cpuFd); }

    // Memoria config
    free(config_Memoria->ALGORITMO_REEMPLAZO);
    free(config_Memoria->PATH_SWAP);
    free(config_Memoria);

    // Memoria log
    log_destroy(log_Memoria);

    rl_clear_history();
}

uint8_t cargar_configuracion(t_config_Memoria* config, t_log* log_Memoria) {
    t_config* cfg = config_create("./cfg/memoria.config");

    if(cfg == NULL) {
        log_error(log_Memoria, "No se encontró memoria.config");
        return 0;
    }

    char* properties[] = {
        "PUERTO_ESCUCHA",
        "TAM_MEMORIA",
        "TAM_PAGINA",
        "ENTRADAS_POR_TABLA",
        "RETARDO_MEMORIA",
        "ALGORITMO_REEMPLAZO",
        "MARCOS_POR_PROCESO",
        "RETARDO_SWAP",
        "PATH_SWAP",
        NULL
    };

    if(!config_has_all_properties(cfg, properties)) {
        log_error(log_Memoria, "Propiedades faltantes en el archivo de configuración");
        config_destroy(cfg);
        return 0;
    }

    config->PUERTO_ESCUCHA = config_get_int_value(cfg, "PUERTO_ESCUCHA");
    config->TAM_MEMORIA = config_get_int_value(cfg, "TAM_MEMORIA");
    config->TAM_PAGINA = config_get_int_value(cfg, "TAM_PAGINA");
    config->ENTRADAS_POR_TABLA = config_get_int_value(cfg, "ENTRADAS_POR_TABLA");
    config->RETARDO_MEMORIA = config_get_int_value(cfg, "RETARDO_MEMORIA");
    config->ALGORITMO_REEMPLAZO = strdup(config_get_string_value(cfg, "ALGORITMO_REEMPLAZO"));
    config->MARCOS_POR_PROCESO = config_get_int_value(cfg, "MARCOS_POR_PROCESO");
    config->PATH_SWAP = strdup(config_get_string_value(cfg, "PATH_SWAP"));


    log_info(log_Memoria, "Archivo de configuración cargado correctamente");

    config_destroy(cfg);

    return 1;
}

