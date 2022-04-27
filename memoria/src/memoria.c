#include "memoria.h"

int main(){
    t_log* logger = log_create("./logs/memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso memoria! %s", mi_funcion_compartida());
    log_destroy(logger);
}