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

// Return timestamp in microseconds
uint64_t get_ms_timestamp() {

    struct timespec tms;

    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    
    // Microseconds = seconds multiplied with 1 million
    uint64_t ms = tms.tv_sec * 1000000;      
    // Add full microseconds
    ms += tms.tv_nsec/1000;    
    // Round up if necessary
    if (tms.tv_nsec % 1000 >= 500) {
        ++ms;
    }

    return ms;
}

// Set timestamp
int set_timestamp(struct timespec* tms) {

    if (clock_gettime(CLOCK_REALTIME, (void*)tms)) {
        return -1;
    }
    return 1;
}

// Compare timestamp and return true if a > b
bool timestamp_cmp(struct timespec* a, struct timespec* b) {
    
    if (a->tv_sec > b->tv_sec)
        return true;
    else if (a->tv_sec == b->tv_sec) 
        return a->tv_nsec > b->tv_nsec;
    else
        return false;

}
