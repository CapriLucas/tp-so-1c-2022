#include "consola.h"

int main(){
    t_log* logger = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso consola! %s", mi_funcion_compartida());
    log_destroy(logger);
}