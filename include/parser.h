#ifndef PARSER_H
#define PARSER_H
#include "aircraft.h"
#include "error.h"

// Legge la risposta JSON di OpenSky e riempie out_array con gli aerei validi.
// Il numero di aerei scritti va in *out_parsed_count.
ErrorCode parser_parse_opensky(const char *json_str, Aircraft *out_array,
                               int max_count, int *out_parsed_count);

#endif