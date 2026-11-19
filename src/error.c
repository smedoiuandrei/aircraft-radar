#include "error.h"

const char *error_to_string(ErrorCode code) {
    switch (code) {
        case ERR_OK:                   return "ERR_OK";
        case ERR_NULL_ARG:             return "ERR_NULL_ARG";
        case ERR_OUT_OF_MEMORY:        return "ERR_OUT_OF_MEMORY";
        case ERR_INVALID_ARG:          return "ERR_INVALID_ARG";
        case ERR_NOT_FOUND:            return "ERR_NOT_FOUND";
        case ERR_NETWORK_TIMEOUT:      return "ERR_NETWORK_TIMEOUT";
        case ERR_NETWORK_UNREACHABLE:  return "ERR_NETWORK_UNREACHABLE";
        case ERR_HTTP_STATUS:          return "ERR_HTTP_STATUS";
        case ERR_JSON_PARSE:           return "ERR_JSON_PARSE";
        case ERR_JSON_MISSING_FIELD:   return "ERR_JSON_MISSING_FIELD";
        case ERR_JSON_INVALID_TYPE:    return "ERR_JSON_INVALID_TYPE";
        case ERR_LIST_FULL:            return "ERR_LIST_FULL";
        case ERR_MUTEX:                return "ERR_MUTEX";
        case ERR_THREAD:               return "ERR_THREAD";
        default:                       return "ERR_UNKNOWN";
    }
}