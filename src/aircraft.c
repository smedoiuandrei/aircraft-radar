#include "aircraft.h"
#include <ctype.h>
#include <math.h>
#include <string.h>

void aircraft_reset(Aircraft *a) {
    if (a == NULL) return;
    memset(a, 0, sizeof(*a));
    a->altitude_m = -1.0f;               // -1 = quota sconosciuta
}

static bool is_blank(const char *s) {
    for (; *s != '\0'; s++) {
        if (!isspace((unsigned char)*s)) return false;
    }
    return true;
}

ErrorCode aircraft_validate(const Aircraft *a) {
    if (a == NULL) return ERR_NULL_ARG;
    if (a->lat < -90.0  || a->lat > 90.0)   return ERR_INVALID_ARG;
    if (a->lon < -180.0 || a->lon > 180.0)  return ERR_INVALID_ARG;
    if (a->icao24[0] == '\0')               return ERR_INVALID_ARG;
    if (is_blank(a->callsign))              return ERR_INVALID_ARG;
    if (a->on_ground)                       return ERR_INVALID_ARG;  // a terra: non serve sul radar
    return ERR_OK;
}

// Formula dell'haversine: distanza sulla sfera tra due punti lat/lon
float aircraft_distance_km(const Aircraft *a, double ref_lat, double ref_lon) {
    const double PI = 3.14159265358979323846;
    const double R_KM = 6371.0;
    double lat1 = ref_lat * PI / 180.0, lat2 = a->lat * PI / 180.0;
    double dlat = lat2 - lat1;
    double dlon = (a->lon - ref_lon) * PI / 180.0;
    double h = sin(dlat / 2) * sin(dlat / 2)
             + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2.0 * atan2(sqrt(h), sqrt(1.0 - h));
    return (float)(R_KM * c);
}

AltitudeBand aircraft_altitude_band(const Aircraft *a) {
    if (a->altitude_m < 3000.0f) return ALT_BAND_LOW;
    if (a->altitude_m < 9000.0f) return ALT_BAND_MEDIUM;
    return ALT_BAND_HIGH;
}