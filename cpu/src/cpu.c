#include "cpu.h"

t_log* mainLog;
t_config_CPU* mainConfig;


static void inicializar_proceso() {
    mainLog = log_create("./logs/cpu.log", "CPU", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_CPU));
    if (!cargar_configuracion(mainConfig, mainLog)) {
        exit(EXIT_FAILURE);
    }
}


int main(){

    inicializar_proceso();

    // Test config.
    printf("ENTRADAS_TLB: %d\n", mainConfig->ENTRADAS_TLB);
    printf("REEMPLAZO_TLB: %s\n", mainConfig->REEMPLAZO_TLB);
    printf("RETARDO_NOOP: %d\n", mainConfig->RETARDO_NOOP);
    printf("IP_MEMORIA: %s\n", mainConfig->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %d\n", mainConfig->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA_DISPATCH: %d\n", mainConfig->PUERTO_ESCUCHA_DISPATCH);
    printf("PUERTO_ESCUCHA_INTERRUPT: %d\n", mainConfig->PUERTO_ESCUCHA_INTERRUPT);

    int cpuFd;
    int memoriaFd;
    int kernelFd;

    // Crear conexión con Memoria   
    memoriaFd = crear_conexion (
        mainLog, 
        "MEMORIA", 
        mainConfig->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

    if (!memoriaFd) {    
        cerrar_programa(mainConfig, mainLog, &memoriaFd);
        return EXIT_FAILURE;
    }

    memoriaFd = iniciar_servidor (
        mainLog,
        "CPU",
        mainConfig->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

/*     
    // Esperar conexión de Kernel
    kernelFd = esperar_cliente (
        mainLog, 
        "KERNEL", 
        cpuFd
    );

    if (!kernelFd) {    
        cerrar_programa(mainConfig, mainLog, &kernelFd);
        return EXIT_FAILURE;
    }
*/

    cerrar_programa(mainConfig, mainLog, &cpuFd);

}
