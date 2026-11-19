#include "network.h"
#include "util.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

struct NetworkClient {
    CURL *handle;
    int   timeout_sec;
};

// libcurl chiama questa funzione ogni volta che arrivano dei byte
static size_t write_response_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    HttpResponse *resp = (HttpResponse *)userp;
    char *p = realloc(resp->body, resp->size + real_size + 1);
    if (p == NULL) return 0;                    // 0 = errore, curl interrompe
    resp->body = p;
    memcpy(resp->body + resp->size, contents, real_size);
    resp->size += real_size;
    resp->body[resp->size] = '\0';
    return real_size;
}

ErrorCode network_global_init(void) {
    return curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK ? ERR_OK : ERR_UNKNOWN;
}
void network_global_cleanup(void) { curl_global_cleanup(); }

NetworkClient *network_client_create(int timeout_sec) {
    NetworkClient *c = util_xmalloc(sizeof(*c));
    c->handle = curl_easy_init();
    if (c->handle == NULL) { free(c); return NULL; }
    c->timeout_sec = timeout_sec;
    return c;
}

void network_client_destroy(NetworkClient *client) {
    if (client == NULL) return;
    curl_easy_cleanup(client->handle);
    free(client);
}

ErrorCode network_get(NetworkClient *client, const char *url, HttpResponse *out) {
    if (client == NULL || url == NULL || out == NULL) return ERR_NULL_ARG;
    out->body = NULL; out->size = 0; out->http_status = 0;

    curl_easy_setopt(client->handle, CURLOPT_URL, url);
    curl_easy_setopt(client->handle, CURLOPT_WRITEFUNCTION, write_response_cb);
    curl_easy_setopt(client->handle, CURLOPT_WRITEDATA, out);
    curl_easy_setopt(client->handle, CURLOPT_TIMEOUT, (long)client->timeout_sec);
    curl_easy_setopt(client->handle, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(client->handle);
    if (res != CURLE_OK) {
        http_response_free(out);                // butto un'eventuale risposta parziale
        return (res == CURLE_OPERATION_TIMEDOUT) ? ERR_NETWORK_TIMEOUT : ERR_NETWORK_UNREACHABLE;
    }
    curl_easy_getinfo(client->handle, CURLINFO_RESPONSE_CODE, &out->http_status);

    if (out->body == NULL) {                    // risposta vuota: do comunque una stringa valida
        out->body = util_xmalloc(1);
        out->body[0] = '\0';
    }
    return ERR_OK;   // OK = la richiesta è avvenuta; che lo status sia 200 lo controlla il chiamante
}

void http_response_free(HttpResponse *response) {
    if (response == NULL) return;
    free(response->body);
    response->body = NULL;
    response->size = 0;
    response->http_status = 0;
}