#ifndef SUI_RENDERER_H
#define SUI_RENDERER_H

#include <stdlib.h>

#include "tgl/tgl.h"

struct sui_cmd;

typedef struct sui_renderer {
    tgl_vao vao;
    tgl_quad rect_vbo;
    tgl_shader rect_shader;
    GLint rect_upos, rect_ucolor;
} sui_renderer;

bool __attribute__((warn_unused_result)) sui_renderer_init(sui_renderer *r, char **error);
void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, struct sui_cmd *cmds, size_t len);

#endif
