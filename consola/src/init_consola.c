#include "init_consola.h"

void cerrar_programa(t_config_consola* cfg, t_log* main_log) {
    log_destroy(main_log);

    free(cfg->IP_KERNEL);
    free(cfg);

    rl_clear_history();
}

uint8_t generar_conexiones(int* kernel_fd, t_config_consola* cfg,t_log* main_log) {
    char* port_kernel = string_itoa(cfg->PUERTO_KERNEL);

    *kernel_fd = crear_conexion(
            main_log,
            "KERNEL",
            cfg->IP_KERNEL,
            port_kernel
    );

    free(port_kernel);

    return *kernel_fd != 0;
}

uint8_t cargar_configuracion(t_config_consola* config, t_log* main_log) {
    t_config* cfg = config_create("./cfg/consola.config");

    if(cfg == NULL) {
        log_error(main_log, "No se encontro consola.config");
        return 0;
    }

    char* properties[] = {
        "IP_KERNEL",
        "PUERTO_KERNEL",
        NULL
    };

    // Falta alguna propiedad?
    //TODO agregar este metodo a utils de shared
    if(!config_has_all_properties(cfg, properties)) {
        log_error(main_log, "Propiedades faltantes en el archivo de configuracion");
        config_destroy(cfg);
        return 0;
    }

    config->IP_KERNEL = strdup(config_get_string_value(cfg, "IP_KERNEL"));
    config->PUERTO_KERNEL = config_get_int_value(cfg, "PUERTO_KERNEL");

    log_info(main_log, "Archivo de configuracion cargado correctamente");

    config_destroy(cfg);

    return 1;
}