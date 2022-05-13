#include "codigo.h"

void enviarInstrucciones(char* texto_crudo,int kernelFd){
	t_paquete* paquete = crear_paquete(ENVIAR_PSEUDO_CODIGO);

	for(uint16_t i = 0; i < 100; i++){
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	instruccion->codigo_instruccion = NO_OP;
	instruccion->parametro = i;
    agregar_a_paquete(paquete,(void*) instruccion, sizeof(t_instruccion));
	free(instruccion);
	}

	enviar_paquete(paquete, kernelFd);
    eliminar_paquete(paquete);
}