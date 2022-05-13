#ifndef TP_2022_1C_PASSIONE_CODIGO_H
#define TP_2022_1C_PASSIONE_CODIGO_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

//Shared
#include "protocolo.h"
#include "shared_utils.h"

void enviarInstrucciones(char* texto_crudo,int kernelFd);
 


#endif