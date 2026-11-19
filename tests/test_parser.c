#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

// due record: il primo valido (callsign con spazi), il secondo a terra (da scartare)
static const char *TWO =
 "{\"time\":1,\"states\":["
 "[\"3c6589\",\"DLH441  \",\"Germany\",1,1,11.5,45.5,10000.0,false,230.0,90.0,0.0,null,10050.0,\"1000\",false,0],"
 "[\"abc123\",\"GND1\",\"Italy\",1,1,9.0,45.0,0.0,true,0.0,0.0,0.0,null,0.0,\"0\",false,0]]}";

static void test_valid_and_filters_on_ground(void) {
    Aircraft out[10]; int n = 0;
    assert(parser_parse_opensky(TWO, out, 10, &n) == ERR_OK);
    assert(n == 1);
    assert(strcmp(out[0].icao24, "3c6589") == 0);
    assert(strcmp(out[0].callsign, "DLH441") == 0);     // spazi tolti
    assert(out[0].lat > 45.49 && out[0].lat < 45.51);
}

static void test_null_altitude_becomes_minus_one(void) {
    const char *j = "{\"states\":[[\"3c6589\",\"DLH441\",\"X\",1,1,11.5,45.5,null,false,230.0,90.0]]}";
    Aircraft out[2]; int n = 0;
    assert(parser_parse_opensky(j, out, 2, &n) == ERR_OK);
    assert(n == 1 && out[0].altitude_m == -1.0f);
}

static void test_null_states_is_empty_not_error(void) {
    Aircraft out[2]; int n = 99;
    assert(parser_parse_opensky("{\"states\":null}", out, 2, &n) == ERR_OK);
    assert(n == 0);
}

static void test_malformed_json(void) {
    Aircraft out[2]; int n = 0;
    assert(parser_parse_opensky("{not json", out, 2, &n) == ERR_JSON_PARSE);
}

static void test_missing_states_field(void) {
    Aircraft out[2]; int n = 0;
    assert(parser_parse_opensky("{}", out, 2, &n) == ERR_JSON_MISSING_FIELD);
}

int main(void) {
    test_valid_and_filters_on_ground();
    test_null_altitude_becomes_minus_one();
    test_null_states_is_empty_not_error();
    test_malformed_json();
    test_missing_states_field();
    printf("Tutti i test del parser sono passati\n");
    return 0;
}