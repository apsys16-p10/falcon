#ifndef DARE_SERVER_H
#define DARE_SERVER_H 

#include "dare_log.h"
#include "dare.h"
#include "../replica-sys/node.h"

struct server_t {
    void *ep;               // endpoint data (network related)
    struct sockaddr_in* peer_address;
};
typedef struct server_t server_t;

struct dare_server_input_t {
    FILE *log;
    peer *peer_pool;
    uint32_t group_size;
    uint32_t *server_idx;
    view* cur_view;
};
typedef struct dare_server_input_t dare_server_input_t;

struct dare_server_data_t {
    dare_server_input_t *input;
    
    view* cur_view;
    
    server_config_t config; // configuration 
    
    dare_log_t  *log;       // local log (remotely accessible)
};
typedef struct dare_server_data_t dare_server_data_t;

/* ================================================================== */

int dare_server_init( dare_server_input_t *input );

#endif /* DARE_SERVER_H */
