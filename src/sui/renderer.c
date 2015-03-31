#include "renderer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <hb-icu.h>

#include "sui/widgets.h"

enum sui_arrattribs {
    SUI_RECT_POS
};

static bool font_fromfont(sui_font *font, sui_renderer *r, char **error, FT_Face face)
{
    (void)r, (void)error;
    font->hb_font = hb_ft_font_create(face, NULL);
    font->hb_face = hb_ft_face_create(face, NULL);
    return true;
}

bool sui_font_fromfile(sui_font *font, sui_renderer *r, char **error, const char *path)
{
    FT_Error fterr;
    if ((fterr = FT_New_Face(r->text.library, path, 0, &font->face))) {
        *error = sui_aprintf("FT_New_Face: Error code %i", fterr);
        return false;
    }
    return font_fromfont(font, r, error, font->face);
}

bool sui_font_fromdata(sui_font *font, sui_renderer *r, char **error, const void *buf, size_t len)
{
    FT_Error fterr;
    if ((fterr = FT_New_Memory_Face(r->text.library, buf, len, 0, &font->face))) {
        *error = sui_aprintf("FT_New_Memory_Face: Error code %i", fterr);
        return false;
    }
    return font_fromfont(font, r, error, font->face);
}

extern const char sui_shader_quad_vert[];
extern const char sui_shader_rect_frag[];
extern const char sui_shader_text_frag[];

bool sui_renderer_init(sui_renderer *r, char **error)
{
    memset(r, 0, sizeof(sui_renderer));
    tgl_vao_init(&r->vao);
    tgl_vao_bind(&r->vao);
    tgl_quad_init(&r->vbo, SUI_RECT_POS);

    {
        struct sui_renderer_rect *rect = &r->rect;
        if (!tgl_make_shader(&rect->shader.vertex, GL_VERTEX_SHADER, sui_shader_quad_vert,
                             strlen(sui_shader_quad_vert), error)) {
            return false;
        }
        if (!tgl_make_shader(&rect->shader.fragment, GL_FRAGMENT_SHADER, sui_shader_rect_frag,
                             strlen(sui_shader_rect_frag), error)) {
            return false;
        }
        rect->shader.program = glCreateProgram();
        glAttachShader(rect->shader.program, rect->shader.vertex);
        glAttachShader(rect->shader.program, rect->shader.fragment);
        if (!tgl_link_program(rect->shader.program, error)) {
            return false;
        }
        rect->upos = glGetUniformLocation(rect->shader.program, "upos");
        rect->ucolor = glGetUniformLocation(rect->shader.program, "ucolor");
    }

    {
        struct sui_renderer_text *text = &r->text;
        if (!tgl_make_shader(&text->shader.vertex, GL_VERTEX_SHADER, sui_shader_quad_vert,
                             strlen(sui_shader_quad_vert), error)) {
            return false;
        }
        if (!tgl_make_shader(&text->shader.fragment, GL_FRAGMENT_SHADER, sui_shader_text_frag,
                             strlen(sui_shader_text_frag), error)) {
            return false;
        }
        text->shader.program = glCreateProgram();
        glAttachShader(text->shader.program, text->shader.vertex);
        glAttachShader(text->shader.program, text->shader.fragment);
        if (!tgl_link_program(text->shader.program, error)) {
            return false;
        }
        text->upos = glGetUniformLocation(text->shader.program, "upos");
        text->ucolor = glGetUniformLocation(text->shader.program, "ucolor");
        text->usampler = glGetUniformLocation(text->shader.program, "usampler");
        FT_Error fterr;
        if ((fterr = FT_Init_FreeType(&text->library))) {
            char *buf = malloc(128);
            int res = snprintf(buf, 128, "FT_Init_FreeType: Error code %i", fterr);
            buf = realloc(buf, res);
            *error = buf;
            return false;
        }
    }

    return true;
}

static void draw_rect(sui_dcmd cmd, unsigned w, unsigned h, sui_renderer *r)
{
    struct sui_renderer_rect *rect = &r->rect;
    glUseProgram(rect->shader.program);
    float ux = cmd.aabb.lx / (float)w;
    float uy = 1.0 - cmd.aabb.ly / (float)h;
    float uw = (cmd.aabb.hx - cmd.aabb.lx) / (float)w;
    float uh = (cmd.aabb.hy - cmd.aabb.ly) / -(float)h;
    glUniform4f(rect->upos, ux, uy, uw, uh);
    unsigned char *col = cmd.wcmd.col;
    glUniform4f(rect->ucolor, col[0] / 255.0, col[1] / 255.0, col[2] / 255.0, col[3] / 255.0);
    tgl_quad_draw_once(&r->vbo);
}

void sui_debug_print_image(const unsigned char *src, unsigned w, unsigned h, unsigned pitch)
{
    static const char chars[] = " .-=+*@#";

    printf("%p %ux%u\n", src, w, h);
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w; x++) {
            unsigned char val = src[y*pitch + x];
            val /= (256/8); // quantize
            putchar(chars[val]);
        }
        putchar('\n');
    }
}

static void simple_blit(unsigned char *dst, const unsigned char *src,
                        unsigned dw, unsigned dh, unsigned sw, unsigned sh,
                        int dx, int dy)
{
    if (sw == 0 || sh == 0) {
        return;
    }
    if (-dx >= (int)sw || -dy >= (int)sh || dx >= (int)dw || dy >= (int)dh) {
        printf("warning: completely clipped src\n");
        return;
    }
    unsigned stride = 1;
    unsigned spitch = sw;
    unsigned dpitch = dw;
    unsigned dxmax = dw - dx;
    unsigned dymax = dh - dy;
    unsigned sx = dx < 0? -dx : 0;
    unsigned sy = dy < 0? -dy : 0;
    unsigned w = dxmax < sw? dxmax : sw;
    unsigned h = dymax < sh? dymax : sh;
#ifndef NDEBUG
    unsigned dend = dw*dh*stride;
    unsigned send = sw*sh*stride;
#endif
    unsigned soffset = 0;
    unsigned doffset = dpitch*dy + stride*dx;
    for (unsigned y = sy; y < h; y++) {
        unsigned dpos = doffset + dpitch*y + stride*sx;
        unsigned spos = soffset + spitch*y + stride*sx;
        unsigned line = w - sx;
        assert(dst+dpos+line <= dst+dend && src+spos+line <= src+send);
        memcpy(dst+dpos, src+spos, line*stride);
    }
}

static void draw_text(sui_dcmd cmd, unsigned w, unsigned h, sui_renderer *r)
{
    struct sui_renderer_text *text = &r->text;
    static const int text_dir_table[] = {
        HB_DIRECTION_LTR,
        HB_DIRECTION_RTL,
        HB_DIRECTION_TTB,
        HB_DIRECTION_BTT
    };

    sui_textfmt fmt = cmd.wcmd.data.text.fmt;
    FT_Face face = fmt.font->face;
    FT_Error fterr;

    if ((fterr = FT_Set_Pixel_Sizes(face, 0, fmt.size))) {
        printf("FT_Set_Char_Size: %i\n", fterr);
        abort();
        return;
    }
    hb_buffer_t *buf = hb_buffer_create();
    hb_buffer_set_unicode_funcs(buf, hb_icu_get_unicode_funcs());
    hb_buffer_set_direction(buf, text_dir_table[fmt.dir]);
    hb_script_t script = hb_script_from_string(fmt.script, -1);
    if (script == HB_SCRIPT_INVALID || script == HB_SCRIPT_UNKNOWN) {
        printf("Invalid script: %s\n", fmt.script);
        abort();
        return;
    }
    hb_buffer_set_script(buf, script);
    hb_language_t lang = hb_language_from_string(fmt.lang, -1);
    if (lang == HB_LANGUAGE_INVALID) {
        printf("Invalid language: %s\n", fmt.lang);
        abort();
        return;
    }
    hb_buffer_set_language(buf, lang);
    const char *msg = cmd.wcmd.data.text.text;
    hb_buffer_add_utf8(buf, msg, strlen(msg), 0, strlen(msg));
    hb_shape(fmt.font->hb_font, buf, NULL, 0);

    unsigned glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

    unsigned str_width = 0, str_height = 0;
    int xadv = 0, yadv = 0;
    for (unsigned i = 0; i < glyph_count; i++) {
        // not a unicode codepoint.
        unsigned codepoint = glyph_info[i].codepoint;
        if ((fterr = FT_Load_Glyph(face, codepoint, FT_LOAD_DEFAULT))) {
            printf("FT_Load_Glyph: %i\n", fterr);
            abort();
            return;
        }
        FT_GlyphSlotRec *glyph = face->glyph;
        if (glyph->format != FT_GLYPH_FORMAT_BITMAP && (fterr = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))) {
            printf("FT_Render_Glyph: %i\n", fterr);
            abort();
            return;
        }
        FT_Bitmap *bm = &glyph->bitmap;
        unsigned top = fmt.size;
        int px = xadv + bm->width*64 - glyph->bitmap_left*64;
        int py = yadv + bm->rows*64 - glyph->bitmap_top*64 + top*64;
        if (px > (int)str_width) {
            str_width = px;
        }
        if (py > (int)str_height) {
            str_height = py;
        }
        xadv += glyph_pos[i].x_advance;
        yadv += glyph_pos[i].y_advance;
    }
    str_width = (str_width + 63) / 64;
    // force the width to be divisible by four
    // for some reason the intel driver freaks the fuck out when you don't do this
    str_width = (str_width+3) & ~3;
    str_height = (str_height + 63) / 64;

    unsigned char *img = calloc(str_width * str_height, 1);
    int pen_x = 0, pen_y = 0;
    for (unsigned i = 0; i < glyph_count; i++) {
        // not a unicode codepoint.
        unsigned codepoint = glyph_info[i].codepoint;
        if ((fterr = FT_Load_Glyph(face, codepoint, FT_LOAD_DEFAULT))) {
            printf("FT_Load_Glyph: %i\n", fterr);
            abort();
            return;
        }
        FT_GlyphSlotRec *glyph = face->glyph;
        if (glyph->format != FT_GLYPH_FORMAT_BITMAP && (fterr = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))) {
            printf("FT_Render_Glyph: %i\n", fterr);
            abort();
            return;
        }
        FT_Bitmap *bm = &glyph->bitmap;
        unsigned top = fmt.size;
        // prevents wrapping to the left side
        if (i == 0) {
            pen_x = glyph->bitmap_left * 64;
        }
        simple_blit(img, bm->buffer,
                    str_width, str_height, bm->width, bm->rows,
                    pen_x/64 - glyph_pos[i].x_offset - glyph->bitmap_left,
                    pen_y/64 - glyph_pos[i].y_offset - glyph->bitmap_top + top);
        pen_x += glyph_pos[i].x_advance;
        pen_y += glyph_pos[i].y_advance;
    }
    hb_buffer_destroy(buf);

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, str_width, str_height, 0, GL_RED, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    free(img);

    glUseProgram(text->shader.program);
    assert(fmt.align == SUI_ALIGN_TOPLEFT && "TODO: Other alignments");
    float ux = cmd.aabb.lx / (float)w;
    float uy = 1.0 - cmd.aabb.ly / (float)h;
    float uw = str_width / (float)w;
    float uh = str_height / -(float)h;
    glUniform4f(text->upos, ux, uy, uw, uh);
    unsigned char *col = cmd.wcmd.col;
    glUniform4f(text->ucolor, col[0] / 255.0, col[1] / 255.0, col[2] / 255.0, col[3] / 255.0);
    glUniform1i(text->usampler, 0);
    tgl_quad_draw_once(&r->vbo);

    glDeleteTextures(1, &tex);
}

void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, sui_dcmd *cmds, size_t len)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tgl_vao_bind(&r->vao);
    for (unsigned i = 0; i < len; i++) {
        switch (cmds[i].wcmd.type) {
        case SUI_RECT:
            draw_rect(cmds[i], w, h, r);
            break;
        case SUI_TEXT:
            draw_text(cmds[i], w, h, r);
            break;
        }
    }
    glDisable(GL_BLEND);
}
