#include "cpu.h"

int main(){
    t_log* logger = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso cpu! %s", mi_funcion_compartida());
    log_destroy(logger);
}