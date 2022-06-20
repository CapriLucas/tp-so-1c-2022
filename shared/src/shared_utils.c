#include "shared_utils.h"

bool config_has_all_properties(t_config* cfg, char** properties) {
    for(uint8_t i = 0; properties[i] != NULL; i++) {
        if(!config_has_property(cfg, properties[i]))
            return false;
    }

    return true;
}

char* leer_archivo_completo(char* path) {
    FILE* fp = fopen(path, "r+");

    if(fp == NULL)
        return NULL;
    
    fseek(fp, 0L, SEEK_END);
    long lSize = ftell(fp);
    rewind(fp);

    char* text = calloc(1, lSize + 1);
    fread(text, lSize, 1, fp);
    fclose(fp);
    
    return text;
}

instruccion_cod get_instruction_cod(char* string){
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

char* get_instruction_name(instruccion_cod cod) {

	switch(cod) {
		case NO_OP:		return "NO_OP";
		case I_O: 		return "I/O";
		case READ:		return "READ";
		case COPY:		return "COPY";
		case WRITE:		return "WRITE";
		case EXIT:		return "EXIT";
		default:		return "";
	}
}
