#include "handler_memory.h"


void handler_memory() {
    
    // Crear conexión con MEMORIA 
    char* puerto_memoria = string_itoa(config_CPU->PUERTO_MEMORIA);
  
    memoriaFd = crear_conexion (
        log_CPU, 
        "CPU", 
        config_CPU->IP_MEMORIA, 
        puerto_memoria
    );

    if (!memoriaFd) {
        log_info(log_CPU, "Error al intentar iniciar conexión con Memoria. Abortando..."); 
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // TEST :: HANDSHAKE message: CPU -> MEMORY
    send_msg_cpu_handshake();
    
    // TEST :: HANDSHAKE response: MEMORY -> CPU 
    uint32_t page_size;
    uint32_t input_table_qty;
    recv_msg_cpu_handshake(&page_size, &input_table_qty);

}

// HANDSHAKE message: CPU -> MEMORY
int send_msg_cpu_handshake() {

    log_info(log_CPU, "Enviando mensaje a MEMORIA: HANDSHAKE");

    t_paquete* paquete = serialize_msg_cpu_mem_handshake();

    enviar_paquete(paquete, memoriaFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;    
}

// HANDSHAKE response: MEMORY -> CPU
int recv_msg_cpu_handshake(uint32_t* page_size, uint32_t* input_table_qty) {

    log_info(log_CPU, "Recibiendo mensaje de MEMORIA: HANDSHAKE");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, memoriaFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_MEM_CPU__HANDSHAKE){
        log_info(log_CPU, "Se espera recibir código de operación: MSG_MEM_CPU__HANDSHAKE");
        return EXIT_FAILURE;
    }

    deserialize_msg_mem_cpu_handshake(paquete, page_size, input_table_qty);
    log_info(log_CPU, "Tamaño de página recibido: %u", *page_size);
    log_info(log_CPU, "Cantidad de entradas por tabla de página recibido: %u", *input_table_qty);

    return EXIT_SUCCESS;
}

// READ message: CPU -> MEMORY
int send_msg_cpu_read(uint32_t* physical_address) {

    log_info(log_CPU, "Enviando mensaje a MEMORIA: READ");

    t_paquete* paquete = serialize_msg_cpu_mem_read(physical_address);

    enviar_paquete(paquete, memoriaFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;    
}

// READ response: MEMORY -> CPU
int recv_msg_cpu_read(uint32_t* value) {

    log_info(log_CPU, "Recibiendo mensaje de MEMORIA: READ");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, memoriaFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_MEM_CPU__READ){
        log_info(log_CPU, "Se espera recibir código de operación: MSG_MEM_CPU__READ");
        return EXIT_FAILURE;
    }

    deserialize_msg_mem_cpu_read(paquete, value);
    log_info(log_CPU, "Valor recibido: %u", *value);

    return EXIT_SUCCESS; 

}

// WRITE message: CPU -> MEMORY
int send_msg_cpu_write(uint32_t* physical_address, uint32_t* value) {
    
    log_info(log_CPU, "Enviando mensaje a MEMORIA: WRITE");

    t_paquete* paquete = serialize_msg_cpu_mem_write(physical_address, value);

    enviar_paquete(paquete, memoriaFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;  
}

// WRITE response: MEMORY -> CPU
int recv_msg_cpu_write() {

    log_info(log_CPU, "Recibiendo mensaje de MEMORIA: WRITE");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, memoriaFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_MEM_CPU__WRITE){
        log_info(log_CPU, "Se espera recibir código de operación: MSG_MEM_CPU__WRITE");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS; 

}

// ACCESS MEMORY message: CPU -> MEMORY
int send_msg_cpu_access_memory_1st(uint32_t* page_number) {

    log_info(log_CPU, "Enviando mensaje a MEMORIA: PAGE_ACCESS_1ST");

    t_paquete* paquete = serialize_msg_cpu_mem_page_access_1st(page_number);

    enviar_paquete(paquete, memoriaFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;  
  
}

// ACCESS MEMORY response: MEMORY -> CPU
int recv_msg_cpu_access_memory_1st(uint32_t* page_number) {

    log_info(log_CPU, "Recibiendo mensaje de MEMORIA: PAGE_ACCESS_1ST");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, memoriaFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_MEM_CPU__PAGE_ACCESS_1ST){
        log_info(log_CPU, "Se espera recibir código de operación: MSG_MEM_CPU__PAGE_ACCESS_1ST");
        return EXIT_FAILURE;
    }

    deserialize_msg_mem_cpu_page_access_1st(paquete, page_number);
    log_info(log_CPU, "Número de página 2do nivel recibida: %u", *page_number);

    return EXIT_SUCCESS;     

}

// ACCESS MEMORY message: CPU -> MEMORY
int send_msg_cpu_access_memory_2nd(uint32_t* page_number) { 

    log_info(log_CPU, "Enviando mensaje a MEMORIA: PAGE_ACCESS_2ND");

    t_paquete* paquete = serialize_msg_mem_cpu_page_access_2nd(page_number);

    enviar_paquete(paquete, memoriaFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;  

}

// ACCESS MEMORY response: MEMORY -> CPU
int recv_msg_cpu_access_memory_2nd(uint32_t* frame_number) {

    log_info(log_CPU, "Recibiendo mensaje de MEMORIA: PAGE_ACCESS_2ND");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, memoriaFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_MEM_CPU__PAGE_ACCESS_1ST){
        log_info(log_CPU, "Se espera recibir código de operación: MSG_MEM_CPU__PAGE_ACCESS_1ST");
        return EXIT_FAILURE;
    }

    deserialize_msg_mem_cpu_page_access_2nd(paquete, frame_number);
    log_info(log_CPU, "Número de marco recibido: %u", *frame_number);

    return EXIT_SUCCESS; 

}

    