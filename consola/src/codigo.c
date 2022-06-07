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

const char* get_instruction_name(instruccion_cod instruccion_cod) {

	switch(instruccion_cod) {
		case NO_OP:		return "NO_OP";
		case I_O: 		return "I/O";
		case READ:		return "READ";
		case COPY:		return "COPY";
		case WRITE:		return "WRITE";
		case EXIT:		return "EXIT";
		default:		return "";
	}
}

void parse_texto_crudo(char* texto_crudo,t_paquete* paquete){
	char** arrayInstrucciones = string_split(texto_crudo, "\n");

	for(int i=0; i< string_array_size(arrayInstrucciones); i++){
		char** instruccion = string_split(arrayInstrucciones[i]," ");
		t_instruccion* aux = malloc(sizeof(t_instruccion));
		aux->codigo_instruccion= mapInstrCodToEnum(instruccion[0]);

		switch(aux->codigo_instruccion) {

			case NO_OP:
				aux->param_1 = atoi(instruccion[1]);
				aux->param_2 = 0;
				break;
			case I_O:
				aux->param_1 = atoi(instruccion[1]);
				aux->param_2 = 0;
				break;
			case READ:
				aux->param_1 = atoi(instruccion[1]);
				aux->param_2 = 0;
				break;
			case COPY:
				aux->param_1 = atoi(instruccion[1]);	
				aux->param_2 = atoi(instruccion[2]);
				break;
			case WRITE:
				aux->param_1 = atoi(instruccion[1]);	
				aux->param_2 = atoi(instruccion[2]);
				break;
			case EXIT:
				aux->param_1 = 0;
				aux->param_2 = 0;
				break;

		}

 		printf("Instrucción #%d\n", i+1);
		printf("Instrucción: %s\n", get_instruction_name(aux->codigo_instruccion));
		printf("Parámetro 1: %d\n", aux->param_1);
		printf("Parámetro 2: %d\n\n", aux->param_2);

		agregar_a_paquete(paquete, (void*) aux, sizeof(t_instruccion));
		string_array_destroy(instruccion);
		free(aux);
	}
	free(texto_crudo);
	string_array_destroy(arrayInstrucciones);
}

void enviarInstrucciones(char* texto_crudo,int kernelFd, uint32_t memory_size){
	t_paquete* paquete = crear_paquete(ENVIAR_PSEUDO_CODIGO);

	// Agregamos el memory_size
	agregar_a_paquete(paquete, &memory_size, sizeof(uint32_t));

	// Agregamos todas las instrucciones
	parse_texto_crudo(texto_crudo, paquete);	
	
	enviar_paquete(paquete, kernelFd);
    eliminar_paquete(paquete);
}

