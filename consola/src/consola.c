#include "consola.h"


int console_cmd(t_console_cmd* cmd, char* argv1, char* argv2, t_log* logger) {

    // Test path: argv[1]
    cmd->PATH = strdup(argv1);
    DIR* dir = opendir(cmd->PATH);
    if (dir) {                      // Path exists.
        closedir(dir);                     
    }
    else if (ENOENT == errno) {     // Path does not exists.
        log_error(logger, "Path %s does not exists.\n", argv1); 
        printf("Path does not exists.\n");
        return EXIT_FAILURE;
    }
    else {                          // Path failed for some reason.
        log_error(logger, "Path %s could not be open.\n", argv1); 
        printf("Path could not be open.\n");      
        return EXIT_FAILURE;
    }

    // Memory size: argv[2]
    if ( (cmd->MEMORY_SIZE = atoi(argv2)) == 0 ) {     // ToDO: validar min & max memory size.
        log_error(logger, "Parámetro memoria inválido: %s\n", argv2); 
        printf("Invalid memory size.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}


int main(int argc, char** argv) {

    // Test argc 
    if (argc < 3) {
        printf("Usage: console <path> <memory size>\n");        
        return EXIT_FAILURE;
    }
    
    t_log* logger = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);

    t_console_cmd* cmd = malloc(sizeof(t_console_cmd));


    if (console_cmd(cmd, argv[1], argv[2], logger) > 0) {
        log_error(logger, "Error al ejecutar console_cmd.\n"); 
        printf("Error al ejecutar console_cmd.\n");
        log_destroy(logger);
        free(cmd);
        return EXIT_FAILURE;
    }


    // Test printf
    log_info(logger, "Path %s and memory size %d.", cmd->PATH, cmd->MEMORY_SIZE);
    printf("Path: %s\n", cmd->PATH);
    printf("Memory size: %d\n", cmd->MEMORY_SIZE);
    

    // Clean up
    log_destroy(logger);
    free(cmd);

    return EXIT_SUCCESS;
}
