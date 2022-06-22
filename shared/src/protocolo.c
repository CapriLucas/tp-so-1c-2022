#include "protocolo.h"


bool send_codigo_op(int fd, op_code cop) {
    size_t size = sizeof(op_code);
    return send(fd,&cop,size,0) != -1;
}

void* serializar_paquete(t_paquete* paquete, uint32_t bytes){
	void * magic = malloc(bytes);
	uint32_t desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void crear_buffer(t_paquete* paquete){
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(op_code codigo_operacion){
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio){
	// Cada vez que agregamos algo a un paquete se le suma un entero que denota su tamanio
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(uint32_t));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(uint32_t));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(uint32_t), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(uint32_t);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente){
	uint32_t bytes = paquete->buffer->size + 2*sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete){
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


/*
 *		HEADER 
 */

// Serializa header
void* serialize_header(uint8_t message, uint32_t payload_size) {
    
    void* stream = malloc(sizeof(uint8_t) + sizeof(uint32_t));
    size_t offset = 0;

    memcpy(stream+offset, &message, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream+offset, &payload_size, sizeof(uint32_t));
   
    return stream;

}


// Deserializa header
void deserialize_header(void* stream, uint8_t* message, uint32_t* payload_size) {

    memcpy(message, stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(payload_size, stream, sizeof(uint32_t));

}


/*
 *		INSTRUCTIONS 
 */

// Serializa estructura t_instruc
void* serialize_instruc(t_instruc* instruc) {
    
    void* stream = malloc(sizeof(t_instruc));
    size_t offset = 0;

    memcpy(stream+offset, &(instruc->instruc_cod), sizeof(t_instruc));
    offset += sizeof(instruc);
    memcpy(stream+offset, &(instruc->param_1), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream+offset, &(instruc->param_2), sizeof(uint32_t));

    return stream;

}


// Deserializa estructura t_instruc
void deserialize_instruc(void* stream, t_instruc* instruc) {

    memcpy(&(instruc->instruc_cod), stream, sizeof(t_instruc));
    stream += sizeof(instruc);
    memcpy(&(instruc->param_1), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(instruc->param_2), stream, sizeof(uint32_t));

}


/*
 *		PCB
 */

// Serializa estructura t_PCB (y su respectiva estructura t_instruc)
void* serialize_pcb(t_PCB* pcb) {

    int size_list = list_size(pcb->l_instruc);
    void* stream = malloc(sizeof(t_PCB) + (sizeof(t_instruc) * size_list));
    size_t offset = 0;

    memcpy(stream+offset, &(pcb->pid), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream+offset, &(pcb->process_size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream+offset, &(pcb->pc), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream+offset, &(pcb->page_table_id), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream+offset, &(pcb->burst_prediction), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    t_instruc* instruc = malloc(sizeof(t_instruc));
    for (int i = 0; i < size_list; i++) {
        
        instruc = list_get(pcb->l_instruc, i);
        void* aux = serialize_instruc(instruc);
        memcpy(stream+offset, aux, sizeof(t_instruc));
        offset += sizeof(t_instruc);
    
    }
    free(instruc);

    return stream;
}


// Deserializa estructura t_PCB (y su respectiva estructura t_instruc)
void deserialize_pcb(void* stream, t_PCB* pcb, size_t size) {

    int size_list = (size - sizeof(t_PCB)) / sizeof(t_instruc);

    memcpy(&(pcb->pid), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pcb->process_size), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pcb->pc), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pcb->page_table_id), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pcb->burst_prediction), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    pcb->l_instruc = list_create(); 
    t_instruc* aux = malloc(sizeof(t_instruc));
    for (int i = 0; i < size_list; i++) {

        t_instruc* instruc = malloc(sizeof(t_instruc));
        memcpy(aux, stream, sizeof(t_instruc));
        deserialize_instruc(aux, instruc);
        list_add(pcb->l_instruc, instruc);
        stream += sizeof(t_instruc);
        
    }
    free(aux);

}


/*
 *		EXEC MSG
 */


// Serializa mensaje EXEC (Kernel -> CPU)
void* serialize_msg_exec(t_PCB* pcb) {

	uint8_t message = MSG_EXEC;
    // Payload size = PCB + lista de instrucciones
    uint32_t payload_size = sizeof(t_PCB) + sizeof(t_instruc) * list_size(pcb->l_instruc);

    // Stream = Header (Message + Payload size) + Payload (PCB + lista de instrucciones)
	void* stream = malloc(sizeof(uint8_t) + sizeof(uint32_t) + payload_size);
	size_t offset = 0;

    memcpy(stream+offset, &message, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream+offset, &payload_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
	memcpy(stream+offset, serialize_pcb(pcb), payload_size);

    return stream;

}


// Deserializa mensaje EXEC (Kernel -> CPU)
void deserialize_msg_exec(void* stream, uint32_t payload_size, t_PCB* pcb) {

	deserialize_pcb(stream, pcb, payload_size);

}


/*
 *		I/O MSG
 */

// Serializa mensaje I/O (CPU -> Kernel)
void* serialize_msg_i_o(uint32_t msec, t_PCB* pcb) {

	uint8_t message = MSG_I_O;
    // Payload size = Miliseconds I/O + PCB + lista de instrucciones
    uint32_t payload_size = sizeof(uint32_t) + sizeof(t_PCB) + sizeof(t_instruc) * list_size(pcb->l_instruc);

    // Stream = Header (Message + Payload size) + Payload (Miliseconds I/O + PCB + lista de instrucciones)
	void* stream = malloc(sizeof(uint8_t) + sizeof(uint32_t) + payload_size);
	size_t offset = 0;

    memcpy(stream+offset, &message, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream+offset, &payload_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
	memcpy(stream+offset, &msec, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream+offset, serialize_pcb(pcb), payload_size - sizeof(uint32_t));

    return stream;
}


// Deserializa mensaje I/O (CPU -> Kernel)
void deserialize_msg_i_o(void* stream, uint32_t payload_size, uint32_t* msec, t_PCB* pcb) {

	memcpy(msec, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

	deserialize_pcb(stream, pcb, payload_size - sizeof(uint32_t));

}


/*
 *		EXIT MSG
 */

// Serializa mensaje EXIT (CPU -> Kernel)
void* serialize_msg_exit(t_PCB* pcb) {

    uint8_t message = MSG_EXIT;
    // Payload size = PCB + lista de instrucciones
    uint32_t payload_size = sizeof(t_PCB) + sizeof(t_instruc) * list_size(pcb->l_instruc);

    // Stream = Header (Message + Payload size) + Payload (PCB + lista de instrucciones)
	void* stream = malloc(sizeof(uint8_t) + sizeof(uint32_t) + payload_size);
	size_t offset = 0;

    memcpy(stream+offset, &message, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream+offset, &payload_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
	memcpy(stream+offset, serialize_pcb(pcb), payload_size);

    return stream;

}


// Deserializa mensaje EXIT (CPU -> Kernel)
void deserialize_msg_exit(void* stream, uint32_t payload_size, t_PCB* pcb) {

	deserialize_pcb(stream, pcb, payload_size);

}
