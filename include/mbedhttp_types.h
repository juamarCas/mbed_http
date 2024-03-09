#ifndef MBED_HTTP_TYPES_H
#define MBED_HTTP_TYPES_H

#include <stdint.h>

#define MBEDHTTP_ERROR -1
#define MBEDHTTP_SUCCESS 1

typedef struct HTTP_response{
    uint16_t status_code;
    char ** headers;
    char * body;
} http_res_t;

typedef struct HTTP_request{
    char ** queries;
} http_req_t;

typedef http_res_t (*method_callback)(http_req_t * req);

typedef enum HTTP_methods{
    HTTP_GET_METOD, 
    HTTP_POST_METHOD,
    HTTP_PUT_METHOD,
    HTTP_DELTE_METHOD
}http_methods_t;

#endif