#include "protocolo.h"


bool send_codigo_op(int fd, op_code cop) {
    size_t size = sizeof(op_code);
    return send(fd,&cop,size,0) != -1;
}