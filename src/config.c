#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AppConfig config_default(void) {
    AppConfig cfg;
    cfg.window_width      = 1000;
    cfg.window_height     = 1000;
    cfg.radar_radius_px   = 420.0f;
    cfg.sweep_speed_rad_s = 1.2f;
    cfg.poll_interval_sec = 12;
    cfg.http_timeout_sec  = 8;
    cfg.bbox_lamin = 44.0;  cfg.bbox_lomin = 8.0;    // Nord Italia
    cfg.bbox_lamax = 47.0;  cfg.bbox_lomax = 13.0;
    cfg.log_level = LOG_LEVEL_INFO;
    return cfg;
}

// Opzioni: --interval=N  --bbox=lamin,lomin,lamax,lomax  --log-level=debug|info|warn|error
bool config_parse_args(AppConfig *cfg, int argc, char **argv) {
    if (cfg == NULL) return false;
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (strncmp(arg, "--interval=", 11) == 0) {
            int v = atoi(arg + 11);
            if (v <= 0) return false;
            cfg->poll_interval_sec = v;
        } else if (strncmp(arg, "--bbox=", 7) == 0) {
            double a, b, c, d;
            if (sscanf(arg + 7, "%lf,%lf,%lf,%lf", &a, &b, &c, &d) != 4) return false;
            cfg->bbox_lamin = a; cfg->bbox_lomin = b;
            cfg->bbox_lamax = c; cfg->bbox_lomax = d;
        } else if (strncmp(arg, "--log-level=", 12) == 0) {
            const char *v = arg + 12;
            if      (strcmp(v, "debug") == 0) cfg->log_level = LOG_LEVEL_DEBUG;
            else if (strcmp(v, "info")  == 0) cfg->log_level = LOG_LEVEL_INFO;
            else if (strcmp(v, "warn")  == 0) cfg->log_level = LOG_LEVEL_WARN;
            else if (strcmp(v, "error") == 0) cfg->log_level = LOG_LEVEL_ERROR;
            else return false;
        } else {
            LOG_WARN("argomento sconosciuto: %s", arg);
        }
    }
    return true;
}