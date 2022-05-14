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


t_console_cmd console_cmd(char** argv, t_log* logger) {

    t_console_cmd cmd;

    // Test path: argv[1]
    cmd.PATH = strdup(argv[1]);

    // Memory size: argv[2]
    if ( (cmd.MEMORY_SIZE = atoi(argv[2])) == 0 ) {     // ToDO: validar min & max memory size.
        log_error(logger, "Parámetro memoria inválido: %s\n", argv[2]); 
        printf("Invalid memory size.\n");
        exit(EXIT_FAILURE);
    }

    return cmd;

}


int main(int argc, char** argv) {

    // Test argc 
    if (argc != 3) {
        printf("Usage: console <path> <memory size>\n");        
        return EXIT_FAILURE;
    }

    initializeProcess();
    
    t_console_cmd cmd = console_cmd(argv, mainLog);

    char* texto_crudo = leer_archivo_completo(cmd.PATH);

    if(texto_crudo == NULL){
        log_error(mainLog,"Fallo al leer el pseudocodigo");
        exit(EXIT_FAILURE);
    }

    int kernelFd;
    if(!generar_conexiones(&kernelFd, mainConfig, mainLog)){
        cerrar_programa(mainConfig, mainLog, &kernelFd);
        return EXIT_FAILURE;
    }

    enviarInstrucciones(texto_crudo, kernelFd);

    cerrar_programa(mainConfig, mainLog, &kernelFd);
    
    return EXIT_SUCCESS;
}
