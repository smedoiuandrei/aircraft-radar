#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_OK = 0,
    ERR_NULL_ARG,
    ERR_OUT_OF_MEMORY,
    ERR_INVALID_ARG,
    ERR_NOT_FOUND,
    ERR_NETWORK_TIMEOUT,
    ERR_NETWORK_UNREACHABLE,
    ERR_HTTP_STATUS,
    ERR_JSON_PARSE,
    ERR_JSON_MISSING_FIELD,
    ERR_JSON_INVALID_TYPE,
    ERR_LIST_FULL,
    ERR_MUTEX,
    ERR_THREAD,
    ERR_UNKNOWN
} ErrorCode;

const char *error_to_string(ErrorCode code);

#endif