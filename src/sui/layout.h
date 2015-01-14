#ifndef SUI_LAYOUT_H
#define SUI_LAYOUT_H

#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "widgets.h"

struct sui_dcmd;

typedef struct sui_lcmd {
    unsigned id, parent;
    float xrel, yrel, wrel, hrel;
    int x, y, w, h;
    struct sui_wcmd cmd;
} sui_lcmd;

bool sui_layout(const sui_lcmd *in, struct sui_dcmd *out, size_t size, unsigned w, unsigned h, char **error) WARN_UNUSED;

#endif
