#ifndef MBED_HTTP_H
#define MBED_HTTP_H

#include <stdint.h>
#include "mbed_http_types.h"

uint8_t add_method_response(const char * method,  const char * endpoint, method_callback);

uint8_t call_method(const char * method, const char * enpoint);


#endif