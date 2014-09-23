#ifndef SUI_RENDERER_H
#define SUI_RENDERER_H

#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include "tgl/tgl.h"

struct sui_cmd;

typedef struct sui_font {
    FT_Face face;
    hb_font_t *hb_font;
    hb_face_t *hb_face;
} sui_font;

typedef struct sui_renderer {
    tgl_vao vao;
    tgl_quad vbo;
    struct sui_renderer_rect {
        tgl_shader shader;
        GLint upos, ucolor;
    } rect;
    struct sui_renderer_text {
        FT_Library library;
        tgl_shader shader;
        GLint upos, ucolor, usampler;
    } text;
} sui_renderer;

bool __attribute__((warn_unused_result)) sui_font_fromfile(sui_font *font, sui_renderer *r, char **error, const char *path);
bool __attribute__((warn_unused_result)) sui_font_fromdata(sui_font *font, sui_renderer *r, char **error, const void *buf, size_t len);

bool __attribute__((warn_unused_result)) sui_renderer_init(sui_renderer *r, char **error);
void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, struct sui_cmd *cmds, size_t len);

#endif
