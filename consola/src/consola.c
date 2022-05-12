#include "consola.h"

t_log* mainLog;
t_config_consola* mainConfig;

static void initializeProcess(){
    mainLog = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_consola));
    if (!cargar_configuracion(mainConfig, mainLog)){
        exit(EXIT_FAILURE);
    }
}

int main(){
    initializeProcess();
    char* text = leer_archivo_completo("./cfg/pseudocodigo"); //TODO este path viene como parte del argc y argv

    if(text == NULL){
        log_error(mainLog,"Fallo al leer el pseudocodigo");
        exit(EXIT_FAILURE);
    }

    cerrar_programa(mainConfig, mainLog);
}