#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>

#define DEG2RAD_F(deg) ((deg) * 0.017453292519943295f)
#define RAD2DEG_F(rad) ((rad) * 57.29577951308232f)

float util_clampf(float value, float min, float max);
float util_deg2rad(float degrees);
float util_rad2deg(float radians);
void  util_safe_strncpy(char *dst, const char *src, size_t dst_size);
void *util_xmalloc(size_t size);
void *util_xcalloc(size_t count, size_t size);

#endif