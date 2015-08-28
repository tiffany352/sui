#ifndef SUI_RENDERER_H
#define SUI_RENDERER_H

#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>
#include <hb-ft.h>

#include "tgl/tgl.h"
#include "util.h"
#include "widgets.h"

typedef struct sui_font {
    FT_Face face;
    hb_font_t *hb_font;
    hb_face_t *hb_face;
} sui_font;

typedef struct sui_layout_format {
    sui_align align;
    sui_direction dir;
    unsigned size;
    const char *lang, *script;
} sui_layout_format;

typedef struct sui_layout {
    hb_buffer_t *buffer;
    unsigned count;
    hb_glyph_info_t *infos;
    hb_glyph_position_t *positions;
} sui_layout;

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

bool sui_layout_init(sui_layout *layout, sui_font *font, const sui_layout_format *fmt,
                     const char *text, size_t length) WARN_UNUSED;

bool sui_font_fromfile(sui_font *font, sui_renderer *r, char **error, const char *path) WARN_UNUSED;
bool sui_font_fromdata(sui_font *font, sui_renderer *r, char **error, const void *buf, size_t len) WARN_UNUSED;

bool sui_renderer_init(sui_renderer *r, char **error) WARN_UNUSED;
void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, struct sui_cmd *cmds, size_t len);

#endif
