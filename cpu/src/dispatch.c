#include "dispatch.h"


// Execute NO_OP
int exec_no_op (t_instruccion* instruction, t_config_CPU* config, t_log* log) {

    printf("EXEC NO_OP\n");
    printf("Instruction param 1 %u\n", instruction->param_1);
    printf("Retardo NO_OP: %u\n", config->RETARDO_NOOP);
    printf("NO_OP: %u\n", config->RETARDO_NOOP * instruction->param_1);

    return EXIT_SUCCESS;

}


// Execute I/O
int exec_i_o (t_instruccion* instruction, t_config_CPU* config, t_log* log) {
    
    printf("EXEC I_O\n");

    return EXIT_SUCCESS;

}


// Execute READ
int exec_read (t_instruccion* instruction, t_config_CPU* config, t_log* log) {
    
    printf("EXEC READ\n");

    return EXIT_SUCCESS;
}


// Execute WRITE
int exec_write (t_instruccion* instruction, t_config_CPU* config, t_log* log) {

    printf("EXEC WRITE\n");

}


// Execute COPY
int exec_copy (t_instruccion* instruction, t_config_CPU* config, t_log* log) {

    printf("EXEC COPY\n");

    return EXIT_SUCCESS;

}


// Execute EXIT
int exec_exit (t_instruccion* instruction, t_config_CPU* config, t_log* log) {

    printf("EXEC EXIT\n");

    return EXIT_SUCCESS;

}