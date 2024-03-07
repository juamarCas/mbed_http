#ifndef MBED_HTTP_TYPES_H
#define MBED_HTTP_TYPES_H

#include <stdint.h>

typedef struct HTTP_response{
    uint16_t status_code;
    char ** headers;
    char * body;
} http_res;

typedef struct HTTP_request{
    char ** queries;
} http_req;

typedef http_res(*method_callback)(http_req * req);


#endif