/*  standard headers  */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <search.h>

/*  custom headers  */
#include "mbedhttp.h"


http_res_t get_callback(http_req_t * req);
http_res_t post_callback(http_req_t * req);



int mbedhttp_init_http_server(uint16_t port);

int main(){
    printf("initt!\n");
    mbedhttp_init_methods();
    
    mbedhttp_add_method_response(HTTP_GET_METOD, "/", get_callback);
    mbedhttp_add_method_response(HTTP_POST_METHOD, "/", post_callback);
    
    mbedhttp_init_http_server(80);
}

int mbedhttp_init_http_server(uint16_t port){
    int iSetOption = 1;
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

    uint8_t _exitloop = 0;

    if(soc == -1){
        perror("socket");
        return -1;
    }

    // invert byte order, this is a requirement of the socaddr_in struct for the port
    uint16_t inv_value = (port >> 8) | (port << 8);

    // remember: port is a 16 bit unsigned number! 
    struct sockaddr_in addr = {
        AF_INET,
        inv_value, /*port 80 in hex inverted*/
        0
    };

    int connection = bind(soc, &addr, sizeof(addr));

    if(connection == -1){
        perror("bind");
        return -1;
    }

    int listener = listen(soc, 10);

    if(listener == -1){
        perror("listen");
        return -1; 
    }


    while (!_exitloop) {
        int client_fd = accept(soc, 0, 0);
        char buff[HTTP_REQUEST_MAX_SIZE] = {0};
        recv(client_fd, buff, HTTP_REQUEST_MAX_SIZE, 0);

        // estract the http method from the petition
        buff[sizeof(buff) - 1] = '\0';
        char * method = strtok(buff, " ");

        if(method == NULL){
            printf("Could not extract the HTTP method from request\n");
            return 1;
        }

        // get the endpoint
        char * enpoint_start = buff + strlen(method) + 1;
        char * endpoint = strtok(enpoint_start, " ");

        if(endpoint == NULL){
            printf("Could not extract the HTTP endpoint from request\n");
            return 1;
        }

        http_methods_t _method = HTTP_GET_METOD;
        if(!strcmp(method, "GET")){
            _method = HTTP_GET_METOD;
        }else if (!strcmp(method, "POST")){
            _method = HTTP_POST_METHOD;
        }
        mbedhttp_call_method(_method, endpoint);
        char * return_val = "{\"nombre\":\"juan\"}";

        char http_response[1024] = {0};
        sprintf(http_response, "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length:%ld\r\n\r\n%s", strlen(return_val), return_val);

        int send_d = send(client_fd, http_response, strlen(http_response), 0);

        close(client_fd);

    }


    close(soc);
    mbedhttp_close_server();

}







http_res_t get_callback(http_req_t * req){
    printf("I am gettingggg\n");
    uint16_t res_code = 200;
    http_res_t res;
    return res;
}


http_res_t post_callback(http_req_t * req){
    printf("I am postingggg\n");
    http_res_t a;
    return a;
}
