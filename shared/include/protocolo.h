#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/log.h>

typedef enum {
    DEBUG               = 69, //TODO ver esto
} op_code;

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
//////  UN SERIALIZAR Y UN DESERIALIZAR POR CADA MENSAJE   //////
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
////// Algunos no necesitan enviar nada en el payload,     //////
////// asi que esos no tienen un serializar/deserializar   //////
////// por implementar. Otros comparten el deserializar    //////
////// con algun otro mensaje.                             //////
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //

//////////// MENSAJES /////////////


// Generico
bool send_codigo_op();

#endif