#include "init_kernel.h"

void cerrar_programa(t_config_kernel* cfg, t_log* mainLog, int* server_fd) {
    log_destroy(mainLog);
    liberar_conexion(server_fd);
    
    free(cfg->IP_MEMORIA);
    free(cfg->IP_CPU);
    free(cfg->ALGORITMO_PLANIFICACION);
    free(cfg);

    rl_clear_history();
}

bool crear_servidor(t_config_kernel* cfg, t_log* logger, int* fd, char* name) {
    char* puerto = string_itoa(cfg->PUERTO_ESCUCHA);
    *fd = iniciar_servidor(
            logger,
            name,
            "0.0.0.0",
            puerto
    );
    free(puerto);
    if (*fd != -1) log_info(logger, "Server listo en KERNEL");
    return *fd != -1;
}

uint8_t cargar_configuracion(t_config_kernel* config, t_log* mainLog) {
    t_config* cfg = config_create("./cfg/kernel.config");

    if(cfg == NULL) {
        log_error(mainLog, "No se encontro kernel.config");
        return 0;
    }

    char* properties[] = {
        "PUERTO_ESCUCHA",    
        "IP_MEMORIA",
        "IP_CPU",
        "PUERTO_MEMORIA",
        "PUERTO_CPU_DISPATCH",
        "PUERTO_CPU_INTERRUPT",
        "ALGORITMO_PLANIFICACION",
        "ESTIMACION_INICIAL",    
        "ALFA",
        "GRADO_MULTIPROGRAMACION",    
        "TIEMPO_MAXIMO_BLOQUEADO",
        NULL
    };

    if(!config_has_all_properties(cfg, properties)) {
        log_error(mainLog, "Propiedades faltantes en el archivo de configuracion");
        config_destroy(cfg);
        return 0;
    }

    config->PUERTO_ESCUCHA = config_get_int_value(cfg,"PUERTO_ESCUCHA");
    config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
    config->IP_CPU = strdup(config_get_string_value(cfg, "IP_CPU"));
    config->PUERTO_MEMORIA = config_get_int_value(cfg,"PUERTO_MEMORIA");    
    config->PUERTO_CPU_DISPATCH = config_get_int_value(cfg,"PUERTO_CPU_DISPATCH");    
    config->PUERTO_CPU_INTERRUPT = config_get_int_value(cfg,"PUERTO_CPU_INTERRUPT");    
    config->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cfg, "ALGORITMO_PLANIFICACION"));
    config->ESTIMACION_INICIAL = config_get_int_value(cfg,"ESTIMACION_INICIAL");    
    config->ALFA = config_get_double_value(cfg,"ALFA");    
    config->GRADO_MULTIPROGRAMACION = config_get_int_value(cfg,"GRADO_MULTIPROGRAMACION");    
    config->TIEMPO_MAXIMO_BLOQUEADO = config_get_int_value(cfg,"TIEMPO_MAXIMO_BLOQUEADO");    

    log_info(mainLog, "Archivo de configuracion cargado correctamente");

    config_destroy(cfg);

    return 1;
}

void initializeKernel(){
    mainLog = log_create("./logs/kernel.log", "KERNEL", true, LOG_LEVEL_INFO);
    uint32_t current_pid = 0;
    initializeSemaphores();
    initializeLists();
    mainConfig = malloc(sizeof(t_config_kernel));
    if (!cargar_configuracion(mainConfig, mainLog) || !crear_servidor(mainConfig, mainLog, &server_fd, SERVERNAME)){
        liberar_conexion(&server_fd);
        exit(EXIT_FAILURE);
    }
}

pthread_mutex_t MUTEX_LISTA_NEW;
sem_t CONTADOR_LISTA_NEW;
pthread_mutex_t MUTEX_CURRENT_PID;

void initializeSemaphores(){
    pthread_mutex_init(&MUTEX_LISTA_NEW, NULL);
    sem_init(&CONTADOR_LISTA_NEW, 0, 0);

    pthread_mutex_init(&MUTEX_CURRENT_PID, NULL);
}

t_list* LISTA_NEW;

void initializeLists(){
    LISTA_NEW = list_create();
}