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

uint8_t recibir_header(t_paquete* paquete, int fd){
    
    paquete->buffer = malloc(sizeof(t_buffer));

    if(recv(fd, &(paquete->codigo_operacion), sizeof(op_code), 0) == 0) {
        return 0;
    }
    
    recv(fd, &(paquete->buffer->size), sizeof(uint32_t), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(fd, paquete->buffer->stream, paquete->buffer->size, 0);

    return 1;
} 

/*
 *		EXEC MSG
 */

// Serializa mensaje EXEC (Kernel -> CPU)
t_paquete* serialize_msg_exec(t_PCB* pcb) {

    t_paquete* paquete = crear_paquete(MSG_EXEC);
    agregar_a_paquete(paquete, pcb, sizeof(t_PCB));

    size_t size_list = list_size(pcb->instructions_list);

    for(int i = 0;  i < size_list; i++) {
		t_instruccion* aux = malloc(sizeof(t_instruccion));
        aux = list_get(pcb->instructions_list, i);
        agregar_a_paquete(paquete, (void*) aux, sizeof(t_instruccion));
        free(aux);
	}
    
    return paquete;
}


// Deserializa mensaje EXEC (Kernel -> CPU)
void deserialize_msg_exec(t_paquete* paquete, t_PCB* pcb) {

    void* stream = paquete->buffer->stream;
    stream += sizeof(uint32_t);
    memcpy(pcb, stream, sizeof(t_PCB));
    stream += sizeof(t_PCB);

    int size_list = (paquete->buffer->size - sizeof(uint32_t) - sizeof(t_PCB)) / (sizeof(t_instruccion) + sizeof(uint32_t));
    
    pcb->instructions_list = list_create();

    for(int i = 0; i < size_list; i++) {

        t_instruccion* aux = malloc(sizeof(t_instruccion));
        stream += sizeof(uint32_t);
        memcpy(aux, stream, sizeof(t_instruccion));
        list_add(pcb->instructions_list, aux);
        stream += sizeof(t_instruccion);

    }
}


/*
 *		I/O MSG
 */

// Serializa mensaje I/O (CPU -> Kernel)
t_paquete* serialize_msg_i_o(uint32_t msec, t_PCB* pcb) {

    t_paquete* paquete = crear_paquete(MSG_I_O);
    agregar_a_paquete(paquete, &msec, sizeof(uint32_t));
    agregar_a_paquete(paquete, pcb, sizeof(t_PCB));

   size_t size_list = list_size(pcb->instructions_list);

    for(int i=0; i < size_list; i++){
		t_instruccion* aux = malloc(sizeof(t_instruccion));
        aux = list_get(pcb->instructions_list, i);
        agregar_a_paquete(paquete, (void*) aux, sizeof(t_instruccion));
        free(aux);
	}

    return paquete;
}

// Deserializa mensaje I/O (CPU -> Kernel)
void deserialize_msg_i_o(t_paquete* paquete, uint32_t* msec, t_PCB* pcb) {

    void* stream = paquete->buffer->stream;
    stream += sizeof(uint32_t);
    memcpy(msec, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    stream += sizeof(uint32_t);
    memcpy(pcb, stream, sizeof(t_PCB));
    stream += sizeof(t_PCB);

    int size_list = (paquete->buffer->size - sizeof(uint32_t) * 3 - sizeof(t_PCB)) / (sizeof(t_instruccion) + sizeof(uint32_t));
    
    pcb->instructions_list = list_create();

    for(int i=0; i < size_list; i++) {

        t_instruccion* aux = malloc(sizeof(t_instruccion));
        stream += sizeof(uint32_t);
        memcpy(aux, stream, sizeof(t_instruccion));
        list_add(pcb->instructions_list, aux);
        stream += sizeof(t_instruccion);

    }
}


/*
 *		EXIT MSG
 */

// Serializa mensaje EXIT (CPU -> Kernel)
t_paquete* serialize_msg_exit(t_PCB* pcb) {

    t_paquete* paquete = crear_paquete(MSG_EXIT);
    agregar_a_paquete(paquete, pcb, sizeof(t_PCB));

    size_t size_list = list_size(pcb->instructions_list);

    for(int i=0;  i < size_list; i++){
		t_instruccion* aux = malloc(sizeof(t_instruccion));
        aux = list_get(pcb->instructions_list, i);
        agregar_a_paquete(paquete, (void*) aux, sizeof(t_instruccion));
        free(aux);
	}
    
    return paquete;
}

// Deserializa mensaje EXIT (CPU -> Kernel)
void deserialize_msg_exit(t_paquete* paquete, t_PCB* pcb) {

    void* stream = paquete->buffer->stream;
    stream += sizeof(uint32_t);
    memcpy(pcb, stream, sizeof(t_PCB));
    stream += sizeof(t_PCB);

    int size_list = (paquete->buffer->size - sizeof(uint32_t) - sizeof(t_PCB)) / (sizeof(t_instruccion) + sizeof(uint32_t));
    
    pcb->instructions_list = list_create();

    for(int i = 0; i < size_list; i++) {

        t_instruccion* aux = malloc(sizeof(t_instruccion));
        stream += sizeof(uint32_t);
        memcpy(aux, stream, sizeof(t_instruccion));
        list_add(pcb->instructions_list, aux);
        stream += sizeof(t_instruccion);

    }

}