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

    // Test load cpu.config -- Borrar
    printf("ENTRADAS_TLB: %u\n", mainConfig->ENTRADAS_TLB);
    printf("REEMPLAZO_TLB: %s\n", mainConfig->REEMPLAZO_TLB);
    printf("RETARDO_NOOP: %u\n", mainConfig->RETARDO_NOOP);
    printf("IP_MEMORIA: %s\n", mainConfig->IP_MEMORIA);
    printf("PUERTO_MEMORIA: %u\n", mainConfig->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA_DISPATCH: %u\n", mainConfig->PUERTO_ESCUCHA_DISPATCH);
    printf("PUERTO_ESCUCHA_INTERRUPT: %u\n", mainConfig->PUERTO_ESCUCHA_INTERRUPT);


    int cpuDispatchFd;
    int cpuInterruptFd;
    int kernelDispatchFd;
    int kernelInterruptFd;
    int memoriaFd;
    

    // Crear conexión con MEMORIA
    memoriaFd = crear_conexion (
        mainLog, 
        "CPU", 
        mainConfig->IP_MEMORIA, 
        "8002" // mainConfig->PUERTO_MEMORIA
    );

    if (!memoriaFd) {    
        cerrar_programa(mainConfig, mainLog, &memoriaFd, NULL);
        return EXIT_FAILURE;
    }

   
    // cpuDispatchFd = server_init(mainLog, "CPU", "8001");
          // mainConfig->PUERTO_ESCUCHA_DISPATCH
    
    // Inicia server en puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    cpuDispatchFd = iniciar_servidor (
        mainLog,
        "CPU",
        "127.0.0.1", 
        "8001"  // mainConfig->PUERTO_ESCUCHA_DISPATCH
    );

    if (!cpuDispatchFd) {    
        cerrar_programa(mainConfig, mainLog, &cpuDispatchFd, NULL);
        return EXIT_FAILURE;
    }

    // Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    cpuInterruptFd = iniciar_servidor (
        mainLog,
        "CPU",
        "127.0.0.1", 
        "8005"  // mainConfig->PUERTO_ESCUCHA_INTERRUPT
    );

    if (!cpuInterruptFd) {    
        cerrar_programa(mainConfig, mainLog, &cpuInterruptFd, NULL);
        return EXIT_FAILURE;
    }

    // Esperar conexión de KERNEL para mensajes de dispatch
    kernelDispatchFd = esperar_cliente (
        mainLog, 
        "KERNEL", 
        cpuDispatchFd
    );

    // Esperar conexión de KERNEL para mensajes de interrupciones
    kernelInterruptFd = esperar_cliente (
        mainLog, 
        "KERNEL", 
        cpuInterruptFd
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
    } */


    cerrar_programa(mainConfig, mainLog, &cpuDispatchFd, &cpuInterruptFd);

}
