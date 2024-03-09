#include "mbedhttp.h"

static HashTable *get_method_table  = NULL;
static HashTable *post_method_table = NULL;

uint8_t mbedhttp_add_method_response(http_methods_t method,  const char * endpoint, http_res_t (*callback)()){

    if(endpoint[0] != '/'){
        printf("The enpoint has to strart with \" / \"\n");
        return 0;
    }

    // some kind of bullshit to store a function as a char array in the hashtable
    char func_buff[sizeof(void *)];
    void * pcallback_fun = (void*) callback;
    memcpy(func_buff, &pcallback_fun, sizeof(void *));


    switch (method)
    {
        case HTTP_GET_METOD:
            ht_insert(get_method_table, (char *)endpoint, func_buff); 
            break;

        case HTTP_POST_METHOD:
            ht_insert(post_method_table, (char *)endpoint, func_buff); 
            break;
    
        default:
            return 0;
            break;
    }

    return 1;
}


void mbedhttp_init_methods(){
    /* one hashtable for each kind of http method */
    get_method_table  = create_table(CAPACITY);
    post_method_table = create_table(CAPACITY);
    
}


void mbedhttp_close_server(){
    free_table(get_method_table);
    free_table(post_method_table);
}



uint8_t mbedhttp_call_method(http_methods_t method, const char * endpoint){

    char * call_back;

     switch (method)
    {
        case HTTP_GET_METOD:
            call_back = ht_search(get_method_table, (char *)endpoint);     
            break;

        case HTTP_POST_METHOD:
            call_back = ht_search(get_method_table, (char *)endpoint); 
            break;
    
        default:
            return 0;
            break;
    }

    if(call_back == NULL){
        //could not find the endpoint
        //todo: implement error 404
        printf("not found that enpoint: %s\n", endpoint);
        return 0;
    }

    // again, some kind of bullshit to get the function i want and calling it
    void * deserialize_func;
    memcpy(&deserialize_func, call_back, sizeof(void *));

    method_callback function = (method_callback)deserialize_func;
    function(NULL);

    
    return 1;
}