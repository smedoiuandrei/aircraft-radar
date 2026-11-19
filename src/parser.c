#include "parser.h"
#include "cJSON.h"
#include "util.h"
#include "log.h"
#include <ctype.h>
#include <string.h>
#include <time.h>

// Ogni "state" di OpenSky è un array posizionale (non un oggetto):
// 0 icao24, 1 callsign, 5 lon, 6 lat, 7 quota baro, 8 on_ground, 9 velocità, 10 heading
// I valori mancanti arrivano come null.

static bool get_string_at(const cJSON *arr, int idx, char *out, size_t out_size) {
    const cJSON *item = cJSON_GetArrayItem(arr, idx);
    if (!cJSON_IsString(item) || item->valuestring == NULL) return false;
    util_safe_strncpy(out, item->valuestring, out_size);
    return true;
}

static bool get_double_at(const cJSON *arr, int idx, double *out) {
    const cJSON *item = cJSON_GetArrayItem(arr, idx);
    if (!cJSON_IsNumber(item)) return false;    // false anche per null
    *out = item->valuedouble;
    return true;
}

static bool get_bool_at(const cJSON *arr, int idx, bool *out) {
    const cJSON *item = cJSON_GetArrayItem(arr, idx);
    if (!cJSON_IsBool(item)) return false;
    *out = cJSON_IsTrue(item);
    return true;
}

// OpenSky riempie i callsign con spazi a destra ("DLH441  ")
static void rtrim(char *s) {
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
}

static ErrorCode parse_single_state_vector(const cJSON *sv, Aircraft *out) {
    if (!cJSON_IsArray(sv)) return ERR_JSON_INVALID_TYPE;
    aircraft_reset(out);

    // campi obbligatori: senza questi il record è inutilizzabile
    if (!get_string_at(sv, 0, out->icao24, sizeof(out->icao24))) return ERR_JSON_MISSING_FIELD;
    if (!get_double_at(sv, 6, &out->lat))                        return ERR_JSON_MISSING_FIELD;
    if (!get_double_at(sv, 5, &out->lon))                        return ERR_JSON_MISSING_FIELD;

    // campi facoltativi: se mancano restano ai valori di aircraft_reset
    get_string_at(sv, 1, out->callsign, sizeof(out->callsign));
    rtrim(out->callsign);

    double v;
    if (get_double_at(sv, 7, &v))  out->altitude_m  = (float)v;
    if (get_double_at(sv, 9, &v))  out->speed_mps   = (float)v;
    if (get_double_at(sv, 10, &v)) out->heading_deg = (float)v;
    bool g;
    if (get_bool_at(sv, 8, &g))    out->on_ground   = g;

    out->last_seen = time(NULL);
    return ERR_OK;
}

ErrorCode parser_parse_opensky(const char *json_str, Aircraft *out_array,
                               int max_count, int *out_parsed_count) {
    if (json_str == NULL || out_array == NULL || out_parsed_count == NULL || max_count <= 0)
        return ERR_NULL_ARG;
    *out_parsed_count = 0;

    cJSON *root = cJSON_Parse(json_str);        // alloca tutto l'albero JSON
    if (root == NULL) {
        LOG_WARN("JSON non valido");
        return ERR_JSON_PARSE;
    }

    const cJSON *states = cJSON_GetObjectItemCaseSensitive(root, "states");
    if (cJSON_IsNull(states)) {                 // nessun aereo nella bbox: non è un errore
        cJSON_Delete(root);
        return ERR_OK;
    }
    if (!cJSON_IsArray(states)) {
        cJSON_Delete(root);
        return ERR_JSON_MISSING_FIELD;
    }

    int count = 0;
    const cJSON *item = NULL;
    cJSON_ArrayForEach(item, states) {
        if (count >= max_count) break;
        Aircraft tmp;
        if (parse_single_state_vector(item, &tmp) != ERR_OK) continue;  // record rotto: lo salto
        if (aircraft_validate(&tmp) != ERR_OK) continue;                // dati non validi: lo salto
        out_array[count++] = tmp;
    }

    cJSON_Delete(root);                         // libero l'albero: unico punto di uscita
    *out_parsed_count = count;
    return ERR_OK;
}