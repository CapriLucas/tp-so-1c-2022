#include "handler_cpu.h"


void handler_cpu() {
    
    // Esperar conexión de CPU
    cpuFd = esperar_cliente (
        log_Memoria, 
        "CPU", 
        memoriaFd
    );

    if (!cpuFd) {        
        log_info(log_Memoria, "Error al intentar iniciar conexión con CPU. Abortando..."); 
        cerrar_programa();
        exit(EXIT_FAILURE);
    }

    // TEST :: HANDSHAKE 
    // Message: CPU -> MEMORY
    recv_msg_cpu_handshake();
    // Response: MEMORY -> CPU
    send_msg_cpu_handshake(&(config_Memoria->TAM_PAGINA), &(config_Memoria->ENTRADAS_POR_TABLA));

    // TEST :: READ 
    // Message: CPU -> MEMORY
    uint32_t physical_address;  
    recv_msg_cpu_read(&physical_address);
    // Response: MEMORY -> CPU
    uint32_t value = 12345;
    send_msg_cpu_read(&value);

    // TEST :: WRITE 
    // Message: CPU -> MEMORY
    recv_msg_cpu_write(&physical_address, &value);
    // Response: MEMORY -> CPU
    send_msg_cpu_write();

    // TEST :: ACCESS MEMORY 
    // Message: CPU -> MEMORY
    uint32_t page_number_1st;
    recv_msg_cpu_access_memory_1st(&page_number_1st);
    // Respose: MEMORY -> CPU
    uint32_t page_number_2nd = 256;
    send_msg_cpu_access_memory_1st(&page_number_2nd);
    // Message: CPU -> MEMORY
    recv_msg_cpu_access_memory_2nd(&page_number_2nd);
    // Response: MEMORY -> CPU
    uint32_t frame_number;
    send_msg_cpu_access_memory_2nd(&frame_number);

}


// HANDSHAKE message: CPU -> MEMORY
int recv_msg_cpu_handshake() {

    log_info(log_Memoria, "Recibiendo mensaje de CPU: HANDSHAKE");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cpuFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_CPU_MEM__HANDSHAKE){
        log_info(log_Memoria, "Se espera recibir código de operación: MSG_CPU_MEM__HANDSHAKE");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    

}

// HANDSHAKE response: MEMORY -> CPU
int send_msg_cpu_handshake(uint32_t* page_size, uint32_t* input_table_qty) {

    log_info(log_Memoria, "Enviando respuesta a CPU: HANDSHAKE");

    t_paquete* paquete = serialize_msg_mem_cpu_handshake(page_size, input_table_qty);

    enviar_paquete(paquete, cpuFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}

// READ message: CPU -> MEMORY
int recv_msg_cpu_read(uint32_t* physical_address) {

    log_info(log_Memoria, "Recibiendo mensaje de CPU: READ");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cpuFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_CPU_MEM__READ){
        log_info(log_Memoria, "Se espera recibir código de operación: MSG_CPU_MEM__READ");
        return EXIT_FAILURE;
    }

    deserialize_msg_cpu_mem_read(paquete, physical_address);
    log_info(log_Memoria, "Dirección física recibida: %u", *physical_address);

    return EXIT_SUCCESS;
}

// READ response: MEMORY -> CPU
int send_msg_cpu_read(uint32_t* value) {

    log_info(log_Memoria, "Enviando respuesta a CPU: READ");

    t_paquete* paquete = serialize_msg_mem_cpu_read(value);

    enviar_paquete(paquete, cpuFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}

// WRITE message: CPU -> MEMORY
int recv_msg_cpu_write(uint32_t* physical_address, uint32_t* value) {

    log_info(log_Memoria, "Recibiendo mensaje de CPU: WRITE");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cpuFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_CPU_MEM__WRITE){
        log_info(log_Memoria, "Se espera recibir código de operación: MSG_CPU_MEM__WRITE");
        return EXIT_FAILURE;
    }

    deserialize_msg_cpu_mem_write(paquete, physical_address, value);
    log_info(log_Memoria, "Dirección física recibida: %u", *physical_address);
    log_info(log_Memoria, "Valor a escribir recibido: %u", *value);

    return EXIT_SUCCESS;    
    
}

// WRITE response: MEMORY -> CPU
int send_msg_cpu_write() {

    log_info(log_Memoria, "Enviando respuesta a CPU: WRITE");

    t_paquete* paquete = serialize_msg_mem_cpu_write();

    enviar_paquete(paquete, cpuFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;
}

// ACCESS MEMORY message: CPU -> MEMORY
int recv_msg_cpu_access_memory_1st(uint32_t* page_number) {

    log_info(log_Memoria, "Recibiendo mensaje de CPU: PAGE_ACCESS_1ST");

    t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cpuFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_CPU_MEM__PAGE_ACCESS_1ST){
        log_info(log_Memoria, "Se espera recibir código de operación: MSG_CPU_MEM__PAGE_ACCESS_1ST");
        return EXIT_FAILURE;
    }

    deserialize_msg_cpu_mem_page_access_1st(paquete, page_number);
    log_info(log_Memoria, "Número de página 1er nivel recibida: %u", *page_number);

    return EXIT_SUCCESS;    
}

// ACCESS MEMORY response: MEMORY -> CPU
int send_msg_cpu_access_memory_1st(uint32_t* page_number) {

    log_info(log_Memoria, "Enviando respuesta a CPU: PAGE_ACCESS_1ST");

    t_paquete* paquete = serialize_msg_mem_cpu_page_access_1st(page_number);

    enviar_paquete(paquete, cpuFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;
}

// ACCESS MEMORY message: CPU -> MEMORY
int recv_msg_cpu_access_memory_2nd(uint32_t* page_number) {

    log_info(log_Memoria, "Recibiendo mensaje de CPU: PAGE_ACCESS_2ND");

        t_paquete* paquete = malloc(sizeof(t_paquete));

    if(recibir_header(paquete, cpuFd) == 0) {
        return EXIT_FAILURE;
    }
    if(paquete->codigo_operacion != MSG_CPU_MEM__PAGE_ACCESS_2ND){
        log_info(log_Memoria, "Se espera recibir código de operación: MSG_CPU_MEM__PAGE_ACCESS_2ND");
        return EXIT_FAILURE;
    }

    deserialize_msg_cpu_mem_page_access_2nd(paquete, page_number);
    log_info(log_Memoria, "Número de página 2do nivel recibida: %u", *page_number);


    return EXIT_SUCCESS;    

}

// ACCESS MEMORY response: MEMORY -> CPU
int send_msg_cpu_access_memory_2nd(uint32_t* frame_number) {

    log_info(log_Memoria, "Enviando respuesta a CPU: PAGE_ACCESS_2ND");

    t_paquete* paquete = serialize_msg_mem_cpu_page_access_2nd(frame_number);

    enviar_paquete(paquete, cpuFd);

    eliminar_paquete(paquete);

    return EXIT_SUCCESS;

}

    

/* 
void handler_cpu(){
    
    // Esperar conexión de CPU
    cpuFd = esperar_cliente (
        log_Memoria, 
        "MEMORIA", 
        memoriaFd,
    );

    while(1){
        t_paquete* paquete = malloc(sizeof(t_paquete));

        if(recibir_header(paquete, cpuFd) == 0){
            return;
        }
        
        switch(paquete->codigo_operacion){
            case HANDSHAKE_CPU:
                crear_proceso(paquete);
                break;
            //TODO implmentar delete y suspend
            default:
                return;
        }
        free(paquete);
    }
} */