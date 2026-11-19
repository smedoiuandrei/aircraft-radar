#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>
#include "log.h"

#define CFG_MAX_AIRCRAFT              200
#define CFG_MAX_CALLSIGN_LEN          16
#define CFG_MAX_CONSECUTIVE_FAILURES  5

typedef struct {
    int    window_width;
    int    window_height;
    float  radar_radius_px;
    float  sweep_speed_rad_s;
    int    poll_interval_sec;
    int    http_timeout_sec;
    double bbox_lamin, bbox_lomin, bbox_lamax, bbox_lomax;
    LogLevel log_level;
} AppConfig;

AppConfig config_default(void);
bool config_parse_args(AppConfig *cfg, int argc, char **argv);

#endif