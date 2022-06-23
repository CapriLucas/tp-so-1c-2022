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

    // Path: argv[1]
    cmd.PATH = strdup(argv[1]);

    // Memory size: argv[2]
    if ((cmd.MEMORY_SIZE = atoi(argv[2])) == 0) {
        log_error(logger, "Parámetro <memory size> inválido: %s\n", argv[2]);
        printf("Parámetro <memory size> inválido.\n");
        exit(EXIT_FAILURE);
    }

    return cmd;
}


int main(int argc, char** argv) {


    if (argc != 3) {
        fprintf(stderr, "Usage: consola <path> <memory size>\n");
        exit(EXIT_FAILURE); 
    }


    // Inicializa log y archivo de configuración
    initializeProcess();


    // Procesa argv: path y memory size
    t_console_cmd cmd = console_cmd(argv, mainLog);


    // Procesa pseudocódigo
    char* texto_crudo = leer_archivo_completo(cmd.PATH);
    if(texto_crudo == NULL){
        log_error(mainLog,"Fallo al leer el pseudocodigo");
        exit(EXIT_FAILURE);
    }


    // Se conecta a KERNEL
    int kernelFd;
    if(!generar_conexiones(&kernelFd, mainConfig, mainLog)){
        cerrar_programa(mainConfig, mainLog, &kernelFd);
        return EXIT_FAILURE;
    }


    // Envía instrucciones
    enviarInstrucciones(texto_crudo, kernelFd, cmd.MEMORY_SIZE);


    // Espera por finalización (mensaje EXIT por parte de Kernel)
    char* msg = malloc(5);
    int recv_bytes = recv(kernelFd, msg, 4, 0);
    msg[4] = '\0';
    printf("Mensaje recibido: %s\n", msg);
    free(msg);


    // Libera recursos y cierra programa
    cerrar_programa(mainConfig, mainLog, &kernelFd);
    return EXIT_SUCCESS;

}
