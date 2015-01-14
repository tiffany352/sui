#include "layout.h"

#include "renderer.h"

int find_id(unsigned id, const sui_lcmd *cmds, size_t n)
{
    for (unsigned i = 0; i < n; i++) {
        if (cmds[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool sui_layout(const sui_lcmd *restrict in, sui_dcmd *restrict out, size_t size, unsigned w, unsigned h, char **error)
{
    sui_aabb *aabbs = calloc(size, sizeof(sui_aabb));
    for (unsigned i = 0; i < size; i++) {
        int idx = 0;
        if (in[i].parent && (idx = find_id(in[i].parent, in, i)) < 0) {
            *error = sui_aprintf("Parent %u for node %u either does not exist or is positioned after current node",
                                 in[i].parent, in[i].id);
            return false;
        }
        sui_aabb p = in[i].parent? aabbs[idx] : (sui_aabb){0,0, w,h};
        unsigned w = p.hx-p.lx, h = p.hy-p.ly;
        unsigned x = p.lx + in[i].xrel * w + in[i].x;
        unsigned y = p.ly + in[i].yrel * h + in[i].y;
        aabbs[i] = (sui_aabb) {
            x, y,
            x + in[i].wrel * w + in[i].w,
            y + in[i].hrel * h + in[i].h
        };
        out[i] = (sui_dcmd){aabbs[i], in[i].cmd};
    }
    free(aabbs);
    return true;
}
