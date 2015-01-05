#include "util.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

extern inline size_t sui_real_reserve(void **ptr, size_t eltsize, size_t size, size_t cap, size_t more);

char *sui_aprintf(const char *fmt, ...)
{
    va_list ap1, ap2;
    va_start(ap1, fmt);
    int len = vsnprintf(NULL, 0, fmt, ap1);
    va_end(ap1);
    if (len < 0) {
        return strdup(strerror(errno));
    }
    char *buf = malloc(len + 1);
    va_start(ap2, fmt);
    vsnprintf(buf, len+1, fmt, ap2);
    va_end(ap2);
    return buf;
}
