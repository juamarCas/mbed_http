
/*  standard headers  */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
//#include <pthread.h>
#include <search.h>

/*  custom headers  */
#include <hash_table.h>

/*  custom typedefs  */
typedef void (*method_callback)(void);

uint8_t add_method_response(const char * method,  const char * endpoint, method_callback);
uint8_t call_method(const char * method, const char * enpoint);

void get_callback(void);
void post_callback(void);


#define HTTP_REQUEST_MAX_SIZE 256

HashTable *get_method_table  = NULL;
HashTable *post_method_table = NULL;

void init_methods();
void init_http_server();

int main(){
    init_http_server();
    add_method_response("GET", "/", get_callback);
    add_method_response("POST", "/", post_callback);
    
    init_http_server();
}

void init_http_server(){
    int iSetOption = 1;
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));


    if(soc == -1){
        perror("socket");
        return -1;
    }

    // remember: port is a 16 bit unsigned number! 
    struct sockaddr_in addr = {
        AF_INET,
        0x5000, /*port 80 in hex inverted*/
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


    while (1) {
        int client_fd = accept(soc, 0, 0);
        char buff[HTTP_REQUEST_MAX_SIZE] = {0};
        recv(client_fd, buff, HTTP_REQUEST_MAX_SIZE, 0);

        // estract the http method from the petition
        char * method = strtok(buff, " ");

        // get the endpoint
        char * enpoint_start = buff + strlen(method) + 1;
        char * endpoint = strtok(enpoint_start, " ");

        call_method(method, endpoint);
        if(!strcmp(method, "GET")){
            printf("I have detected a get method\n");
        }else if (!strcmp(method, "POST")){
            printf("I have detected a POST method\n");
        }
        char * return_val = "{\"nombre\":\"juan\"}";

        char http_response[1024] = {0};
        sprintf(http_response, "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length:%ld\r\n\r\n%s", strlen(return_val), return_val);

        int send_d = send(client_fd, http_response, strlen(http_response), 0);

        close(client_fd);

    }


    close(soc);
    free_table(get_method_table);
    free_table(post_method_table);
}


void init_methods(){
    /* one hashtable for each kind of http method */
    get_method_table  = create_table(CAPACITY);
    post_method_table = create_table(CAPACITY);
    
}


uint8_t add_method_response(const char * method,  const char * endpoint, void (*callback)()){

    if(strcmp(endpoint[0], "/")){
        printf("The enpoint has to strart with \" / \"\n");
        return 0;
    }
    // some kind of bullshit to store a function as a char array in the hashtable
    char func_buff[sizeof(void *)];
    void * pcallback_fun = (void*) callback;
    memcpy(func_buff, &pcallback_fun, sizeof(void *));

    if(!strcmp(method, "GET")){
        ht_insert(get_method_table, (char *)endpoint, func_buff);       
    }else if (!strcmp(method, "POST")){
        ht_insert(post_method_table, (char *)endpoint, func_buff); 
    }else{
        //could not find method
        return 0;
    }

    return 1;
}

uint8_t call_method(const char * method, const char * endpoint){

    char * call_back;

    if(!strcmp(method, "GET")){
        call_back = ht_search(get_method_table, (char *)endpoint);       
    }else if (!strcmp(method, "POST")){
        call_back = ht_search(get_method_table, (char *)endpoint); 
    }

    if(call_back == NULL){
        //could not find the endpoint
        //todo: implement error 404
        printf("not found that enpoint: %s\n", endpoint);
        return 2;
    }

    // again, some kind of bullshit to get the function i want and calling it
    void * deserialize_func;
    memcpy(&deserialize_func, call_back, sizeof(void *));

    method_callback function = (method_callback)deserialize_func;
    function();

    
    return 1;
}

void get_callback(){
    printf("I am gettingggg\n");
}


void post_callback(){
    printf("I am postingggg\n");
}
