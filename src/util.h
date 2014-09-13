#ifndef SUI_UTIL_H
#define SUI_UTIL_H

#include <stdlib.h>
#include <string.h>

inline size_t sui_real_reserve(void **ptr, size_t eltsize, size_t size, size_t cap, size_t more)
{
    if (cap - size < more) {
        size_t newcap = cap>8? cap*2 : 16;
        void *newptr = malloc(eltsize * newcap);
        if (*ptr) {
            memcpy(newptr, *ptr, eltsize * size);
            free(*ptr);
        }
        memset((char*)newptr + eltsize * size, 0, eltsize * (newcap - size));
        *ptr = newptr;
        return newcap;
    }
    return cap;
}

#define sui_reserve(p, s, c, m) sui_real_reserve((void**)&p, sizeof(p[0]), s, c, m)

#endif
