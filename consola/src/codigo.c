#include "codigo.h"

instruccion_cod mapInstrCodToEnum(char* string){
	if(strcmp(string, "NO_OP") == 0){
		return NO_OP;
	}
	if(strcmp(string,"I/O") == 0){
		return I_O;
	}
	if(strcmp(string,"WRITE") == 0){
		return WRITE;
	}
	if(strcmp(string,"COPY") == 0){
		return COPY;
	}
	if(strcmp(string,"READ") == 0){
		return READ;
	}
	if(strcmp(string,"EXIT") == 0){
		return EXIT;
	}
	return -1;
}

void parse_texto_crudo(char* texto_crudo,t_paquete* paquete){
	char** arrayInstrucciones = string_split(texto_crudo, "\n");

	for(int i=0; i< string_array_size(arrayInstrucciones); i++){
		char** instruccion = string_split(arrayInstrucciones[i]," ");
		t_instruccion* aux = malloc(sizeof(t_instruccion));
		aux->codigo_instruccion= mapInstrCodToEnum(instruccion[0]);
		if(aux->codigo_instruccion == EXIT){
			aux->parametro = 0;
		} else {
			aux->parametro = atoi(instruccion[1]);
		}

		agregar_a_paquete(paquete, (void*) aux, sizeof(t_instruccion));
		string_array_destroy(instruccion);
		free(aux);
	}
	free(texto_crudo);
	string_array_destroy(arrayInstrucciones);
}

void enviarInstrucciones(char* texto_crudo,int kernelFd){
	t_paquete* paquete = crear_paquete(ENVIAR_PSEUDO_CODIGO);
	parse_texto_crudo(texto_crudo, paquete);
	enviar_paquete(paquete, kernelFd);
    eliminar_paquete(paquete);
}

