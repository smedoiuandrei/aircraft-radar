#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static LogLevel g_min_level = LOG_LEVEL_INFO;

static const char *level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        default:              return "?";
    }
}

void log_init(LogLevel min_level)      { g_min_level = min_level; }
void log_set_level(LogLevel min_level) { g_min_level = min_level; }
void log_shutdown(void)                { }

void log_write(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < g_min_level) return;      // sotto la soglia: non stampo

    char ts[16] = "??:??:??";
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (tm_info != NULL) strftime(ts, sizeof(ts), "%H:%M:%S", tm_info);

    fprintf(stderr, "[%s] [%s] %s:%d: ", ts, level_to_string(level), file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);          // la parte "printf" del messaggio
    va_end(args);
    fputc('\n', stderr);
}