#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include <pthread.h>

#include "global.h"
#include "config.h"
#include "init.h"
#include "dispatch.h"
#include "interrupt.h"
#include "handler_memory.h"
#include "tlb.h"

// Shared
#include "protocolo.h"
#include "sockets.h"
#include "shared_utils.h"

#endif
