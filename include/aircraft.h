#ifndef AIRCRAFT_H
#define AIRCRAFT_H
#include <stdbool.h>
#include <time.h>
#include "error.h"
#include "config.h"

typedef enum {
    ALT_BAND_LOW,       // < 3000 m
    ALT_BAND_MEDIUM,    // 3000 - 9000 m
    ALT_BAND_HIGH       // > 9000 m
} AltitudeBand;

typedef struct { float x, y; } ScreenPos;

typedef struct {
    char   icao24[8];                    // id del transponder, es. "3c6589"
    char   callsign[CFG_MAX_CALLSIGN_LEN];
    double lat, lon;                     // gradi decimali
    float  altitude_m;                   // metri, -1 se sconosciuta
    float  speed_mps;                    // m/s
    float  heading_deg;                  // 0-360
    bool   on_ground;
    time_t last_seen;
    ScreenPos screen_pos;                // lo calcola il radar, non il parser
} Aircraft;

void         aircraft_reset(Aircraft *a);
ErrorCode    aircraft_validate(const Aircraft *a);
float        aircraft_distance_km(const Aircraft *a, double ref_lat, double ref_lon);
AltitudeBand aircraft_altitude_band(const Aircraft *a);

#endif