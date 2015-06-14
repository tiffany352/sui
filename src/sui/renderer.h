#ifndef SUI_RENDERER_H
#define SUI_RENDERER_H

#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include "tgl/tgl.h"
#include "util.h"

struct sui_cmd;

typedef struct sui_font {
    FT_Face face;
    hb_font_t *hb_font;
    hb_face_t *hb_face;
    GLuint tex;
    unsigned width, height;
    unsigned char *data;
    FT_Glyph_Metrics *metrics;
    uint32_t *codepoints;
    size_t length, capacity;
    // TODO Index of codepoint -> glyph
} sui_font;

typedef enum sui_align {
    SUI_ALIGN_LEFT   = 0,
    SUI_ALIGN_RIGHT  = 1,
    SUI_ALIGN_TOP    = 2,
    SUI_ALIGN_BOTTOM = 3,
    SUI_ALIGN_TOPLEFT = SUI_ALIGN_TOP|SUI_ALIGN_LEFT,
    SUI_ALIGN_TOPRIGHT = SUI_ALIGN_TOP|SUI_ALIGN_RIGHT,
    SUI_ALIGN_BOTTOMLEFT = SUI_ALIGN_BOTTOM|SUI_ALIGN_LEFT,
    SUI_ALIGN_BOTTOMRIGHT = SUI_ALIGN_BOTTOM|SUI_ALIGN_RIGHT,
} sui_align;

typedef enum sui_direction {
    SUI_DIR_LTR,
    SUI_DIR_RTL,
    SUI_DIR_TTB,
    SUI_DIR_BTT,
} sui_direction;

typedef struct sui_textfmt {
    sui_align align;
    sui_direction dir;
    unsigned size;
    const char *lang, *script;
} sui_textfmt;

typedef struct sui_layout {
    hb_buffer_t *buf;
    unsigned glyph_count;
    hb_glyph_info_t *glyph_info;
    hb_glyph_position_t *glyph_pos;
    sui_font *font;
    sui_align align;
    // 26.6 fixed
    uint32_t width, height;
    uint32_t origin_x, origin_y;
} sui_layout;

typedef struct sui_renderer {
    tgl_vao vao;
    tgl_quad vbo;
    struct sui_renderer_rect {
        tgl_shader shader;
        GLint upos, umat, ucolor;
    } rect;
    struct sui_renderer_text {
        FT_Library library;
        tgl_shader shader;
        GLint upos, umat, ucolor, usampler, uchar, usize;
    } text;
} sui_renderer;

bool sui_font_fromfile(sui_font *font, sui_renderer *r, char **error, const char *path) WARN_UNUSED;
bool sui_font_fromdata(sui_font *font, sui_renderer *r, char **error, const void *buf, size_t len) WARN_UNUSED;
unsigned sui_font_getGlyph(sui_font *font, unsigned codepoint);
bool sui_font_layout(sui_font *font, sui_layout *layout, sui_textfmt *fmt, const char *text, size_t len);

bool sui_renderer_init(sui_renderer *r, char **error) WARN_UNUSED;
// Identity matrix if null
void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, struct sui_cmd *cmds, size_t len, const float *matrix);

#endif
