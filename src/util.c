#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float util_clampf(float value, float min, float max) {
    if (min > max) {            // intervallo al contrario: li scambio
        float temp = min;
        min = max;
        max = temp;
    }
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float util_deg2rad(float degrees) { return DEG2RAD_F(degrees); }
float util_rad2deg(float radians) { return RAD2DEG_F(radians); }

// strncpy da sola non garantisce il '\0' finale: lo metto io
void util_safe_strncpy(char *dst, const char *src, size_t dst_size) {
    if (dst == NULL || src == NULL || dst_size == 0) return;
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

// malloc che non restituisce mai NULL: se la memoria finisce, chiude il programma
void *util_xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "out of memory, size=%zu\n", size);
        abort();
    }
    return ptr;
}

void *util_xcalloc(size_t count, size_t size) {
    void *ptr = calloc(count, size);
    if (ptr == NULL) {
        fprintf(stderr, "out of memory, count=%zu size=%zu\n", count, size);
        abort();
    }
    return ptr;
}