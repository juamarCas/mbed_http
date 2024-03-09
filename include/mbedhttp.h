/**
 * @author Juan D. Martín
 * @version v0.0.1
*/

#ifndef MBED_HTTP_H
#define MBED_HTTP_H

#define HTTP_REQUEST_MAX_SIZE 256

#include <stdint.h>
#include "mbedhttp_types.h"
#include "hash_table.h"


/**
 * @brief adds a endpoint to a called method
 * @param method enum indicating the type of method to be added
 * @param enpoint string that represents the enpoint to be added
 * @param callback function that is called when the enpoint is detected
 * @return 1 if added correctly, 0 if any error happened
*/
uint8_t mbedhttp_add_method_response(http_methods_t method,  const char * endpoint, method_callback callback);
uint8_t mbedhttp_call_method(http_methods_t method, const char * enpoint);

void mbedhttp_init_methods();
void mbedhttp_close_server();

#endif