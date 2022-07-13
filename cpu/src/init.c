#include "init.h"


void inicializar_proceso() {

    // CPU log
    log_CPU = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    
    // CPU config
    config_CPU = malloc(sizeof(t_config_CPU));
    if (!cargar_configuracion(config_CPU, log_CPU)) {
        exit(EXIT_FAILURE);
    }

    // Mutex Interrupt
    pthread_mutex_init(&MUTEX_INTERRUPT, NULL);
}


void cerrar_programa() {

    // Sockets
    if (memoriaFd) { liberar_conexion(&memoriaFd); }   
    if (cpuDispatchFd) { liberar_conexion(&cpuDispatchFd); }   
    if (kernelDispatchFd) { liberar_conexion(&kernelDispatchFd); }   
    if (cpuInterruptFd) { liberar_conexion(&cpuInterruptFd); }   
    if (kernelInterruptFd) { liberar_conexion(&kernelInterruptFd); }   

    // CPU config    
    free(config_CPU->REEMPLAZO_TLB);
    free(config_CPU->IP_MEMORIA);
    free(config_CPU);

    // Mutex Interrupt
    pthread_mutex_destroy(&MUTEX_INTERRUPT);

    // CPU log
    log_destroy(log_CPU);

    rl_clear_history();
}


uint8_t cargar_configuracion(t_config_CPU* config, t_log* mainLog) {
    t_config* cfg = config_create("./cfg/cpu.config");

    if(cfg == NULL) {
        log_error(mainLog, "No se encontró cpu.config");
        return 0;
    }

    char* properties[] = {
        "ENTRADAS_TLB",
        "REEMPLAZO_TLB",
        "RETARDO_NOOP",
        "IP_MEMORIA",
        "PUERTO_MEMORIA",
        "PUERTO_ESCUCHA_DISPATCH",
        "PUERTO_ESCUCHA_INTERRUPT",
        NULL
    };

    if(!config_has_all_properties(cfg, properties)) {
        log_error(mainLog, "Propiedades faltantes en el archivo de configuración");
        config_destroy(cfg);
        return 0;
    }

    config->ENTRADAS_TLB = config_get_int_value(cfg, "ENTRADAS_TLB");
    config->REEMPLAZO_TLB = strdup(config_get_string_value(cfg, "REEMPLAZO_TLB"));
    config->RETARDO_NOOP = config_get_int_value(cfg, "RETARDO_NOOP");
    config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
    config->PUERTO_MEMORIA = config_get_int_value(cfg, "PUERTO_MEMORIA");
    config->PUERTO_ESCUCHA_DISPATCH = config_get_int_value(cfg, "PUERTO_ESCUCHA_DISPATCH");
    config->PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(cfg, "PUERTO_ESCUCHA_INTERRUPT");

    log_info(mainLog, "Archivo de configuración cargado correctamente");

    config_destroy(cfg);

    return 1;
}