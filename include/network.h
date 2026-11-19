#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>
#include "error.h"

typedef struct {
    char  *body;          // risposta, terminata da '\0' (la libera il chiamante)
    size_t size;
    long   http_status;   // 200, 429, 500...
} HttpResponse;

typedef struct NetworkClient NetworkClient;   // opaco: i dettagli stanno in network.c

ErrorCode network_global_init(void);
void      network_global_cleanup(void);

NetworkClient *network_client_create(int timeout_sec);
void           network_client_destroy(NetworkClient *client);

ErrorCode network_get(NetworkClient *client, const char *url, HttpResponse *out_response);
void      http_response_free(HttpResponse *response);

#endif