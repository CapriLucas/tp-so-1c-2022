#include "init_consola.h"

uint8_t cargar_configuracion(t_config_consola* config, t_log* mainLog) {
    t_config* cfg = config_create("./cfg/consola.config");

    if(cfg == NULL) {
        log_error(mainLog, "No se encontro consola.config");
        return 0;
    }

    char* properties[] = {
        "IP_KERNEL",
        "PUERTO_KERNEL",
        NULL
    };

    if(!config_has_all_properties(cfg, properties)) {
        log_error(mainLog, "Propiedades faltantes en el archivo de configuracion");
        config_destroy(cfg);
        return 0;
    }

    config->IP_KERNEL = strdup(config_get_string_value(cfg, "IP_KERNEL"));
    config->PUERTO_KERNEL = config_get_int_value(cfg, "PUERTO_KERNEL");

    log_info(mainLog, "Archivo de configuracion cargado correctamente");

    config_destroy(cfg);

    return 1;
}

void cerrar_programa(t_config_consola* cfg, t_log* mainLog) {
    log_destroy(mainLog);

    free(cfg->IP_KERNEL);
    free(cfg);

    rl_clear_history();
}