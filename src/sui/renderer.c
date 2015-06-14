#include "renderer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <harfbuzz/hb-icu.h>

#include "sui/widgets.h"

enum sui_arrattribs {
    SUI_RECT_POS
};

static const unsigned glyph_size = 128;

static bool font_fromfont(sui_font *font, sui_renderer *r, char **error, FT_Face face)
{
    (void)r, (void)error;
    font->face = face;
    FT_Error fterr;
    if ((fterr = FT_Set_Pixel_Sizes(font->face, 0, glyph_size))) {
        *error = sui_aprintf("FT_Set_Char_Size: %i\n", fterr);
        return false;
    }
    font->hb_font = hb_ft_font_create(face, NULL);
    font->hb_face = hb_ft_face_create(face, NULL);
    font->width = (font->face->bbox.xMax - font->face->bbox.xMin + 63) * 64 / font->face->units_per_EM;
    // force the width to be divisible by four, because of alignment
    font->width = (font->width+3)&~3;
    font->height = (font->face->bbox.yMax - font->face->bbox.yMin + 63) * 64 / font->face->units_per_EM;
    glGenTextures(1, &font->tex);
    return true;
}

bool sui_font_fromfile(sui_font *font, sui_renderer *r, char **error, const char *path)
{
    memset(font, 0, sizeof(sui_font));
    FT_Error fterr;
    if ((fterr = FT_New_Face(r->text.library, path, 0, &font->face))) {
        *error = sui_aprintf("FT_New_Face: Error code %i", fterr);
        return false;
    }
    return font_fromfont(font, r, error, font->face);
}

bool sui_font_fromdata(sui_font *font, sui_renderer *r, char **error, const void *buf, size_t len)
{
    memset(font, 0, sizeof(sui_font));
    FT_Error fterr;
    if ((fterr = FT_New_Memory_Face(r->text.library, buf, len, 0, &font->face))) {
        *error = sui_aprintf("FT_New_Memory_Face: Error code %i", fterr);
        return false;
    }
    return font_fromfont(font, r, error, font->face);
}

unsigned sui_font_getGlyph(sui_font *font, unsigned codepoint)
{
    for (unsigned i = 0; i < font->length; i++) {
        if (font->codepoints[i] == codepoint) {
            return i;
        }
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, font->tex);
    if (font->length >= font->capacity) {
        size_t new_cap = font->capacity? font->capacity * 3 : 64;
        unsigned char *data = calloc(font->width * font->height * new_cap, 1);
        if (font->data) {
            memcpy(data, font->data, font->width * font->height * font->length * 1);
            free(font->data);
        }
        font->data = data;
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8,
                     font->width, font->height, new_cap,
                     0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                        0, 0, 0,
                        font->width, font->height, font->length,
                        GL_RED, GL_UNSIGNED_BYTE, font->data);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -1.0);
        FT_Glyph_Metrics *metrics = calloc(new_cap, sizeof(FT_Glyph_Metrics));
        if (font->metrics) {
            memcpy(metrics, font->metrics, font->length * sizeof(FT_Glyph_Metrics));
            free(font->metrics);
        }
        font->metrics = metrics;
        uint32_t *codepoints = calloc(new_cap, sizeof(uint32_t));
        if (font->codepoints) {
            memcpy(codepoints, font->codepoints, font->length * sizeof(uint32_t));
            free(font->codepoints);
        }
        font->codepoints = codepoints;
        font->capacity = new_cap;
    }
    FT_Error fterr;
    if ((fterr = FT_Set_Pixel_Sizes(font->face, 0, glyph_size))) {
        printf("FT_Set_Char_Size: %i\n", fterr);
        abort();
    }
    if ((fterr = FT_Load_Glyph(font->face, codepoint, FT_LOAD_DEFAULT | FT_LOAD_NO_SCALE))) {
        printf("FT_Load_Glyph: %i\n", fterr);
        abort();
    }
    FT_GlyphSlotRec *glyph = font->face->glyph;
    if (font->face->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        printf("Bitmap fonts are not supported.");
        abort();
    }
    if ((fterr = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))) {
        printf("FT_Render_Glyph: %i\n", fterr);
        abort();
    }
    unsigned l = font->length++;
    font->codepoints[l] = codepoint;
    font->metrics[l] = glyph->metrics;
    FT_Bitmap *bitmap = &glyph->bitmap;
    unsigned char *ldata = font->data + font->width*font->height*l;
    assert(font->height >= bitmap->rows);
    assert(font->width >= bitmap->width);
    unsigned diff = 0; //font->height - bitmap->rows;
    for (unsigned i = 0; i < bitmap->rows; i++) {
        memcpy(ldata + font->width*(i+diff), bitmap->buffer + bitmap->pitch*i, bitmap->width);
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    0, 0, 0,
                    font->width, font->height, font->length,
                    GL_RED, GL_UNSIGNED_BYTE, font->data);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    return l;
}

typedef struct sui_iaabb {
    int lx, ly;
    int hx, hy;
} sui_iaabb;

#define min(a,b) (a>b?b:a)
#define max(a,b) (a>b?a:b)

static sui_iaabb sui_iaabb_expand(sui_iaabb a, sui_iaabb b)
{
    sui_iaabb r;
    r.lx = min(a.lx, b.lx);
    r.ly = min(a.ly, b.ly);
    r.hx = max(a.hx, b.hx);
    r.hy = max(a.hy, b.hy);
    return r;
}

bool sui_font_layout(sui_font *font, sui_layout *layout, sui_textfmt *fmt, const char *text, size_t len)
{
    static const int text_dir_table[] = {
        HB_DIRECTION_LTR,
        HB_DIRECTION_RTL,
        HB_DIRECTION_TTB,
        HB_DIRECTION_BTT
    };

    layout->font = font;
    layout->align = fmt->align;

    layout->buf = hb_buffer_create();
    hb_buffer_set_unicode_funcs(layout->buf, hb_icu_get_unicode_funcs());
    hb_buffer_set_direction(layout->buf, text_dir_table[fmt->dir]);
    hb_script_t script = hb_script_from_string(fmt->script, -1);
    if (script == HB_SCRIPT_INVALID || script == HB_SCRIPT_UNKNOWN) {
        printf("Invalid script: %s\n", fmt->script);
        abort();
        return false;
    }
    hb_buffer_set_script(layout->buf, script);
    hb_language_t lang = hb_language_from_string(fmt->lang, -1);
    if (lang == HB_LANGUAGE_INVALID) {
        printf("Invalid language: %s\n", fmt->lang);
        return false;
    }
    hb_buffer_set_language(layout->buf, lang);
    hb_buffer_add_utf8(layout->buf, text, len, 0, len);
    hb_shape(font->hb_font, layout->buf, NULL, 0);

    layout->glyph_info = hb_buffer_get_glyph_infos(layout->buf, &layout->glyph_count);
    layout->glyph_pos = hb_buffer_get_glyph_positions(layout->buf, &layout->glyph_count);

    sui_iaabb bb = (sui_iaabb){0,0,0,0};
    int xadv = 0, yadv = 0;
    for (unsigned i = 0; i < layout->glyph_count; i++) {
        // Not a unicode codepoint
        unsigned codepoint = layout->glyph_info[i].codepoint;
        unsigned id = sui_font_getGlyph(font, codepoint);
        FT_Glyph_Metrics *metrics = &font->metrics[id];
        int px = xadv + metrics->horiBearingX;
        int py = yadv + metrics->horiBearingY;
        bb = sui_iaabb_expand(bb, (sui_iaabb){px, py - metrics->height, px + metrics->width, py});
        xadv += layout->glyph_pos[i].x_advance;
        yadv += layout->glyph_pos[i].y_advance;
    }
    layout->origin_x = -bb.lx;
    layout->origin_y = -bb.ly;
    layout->width = bb.hx - bb.lx;
    layout->height = bb.hy - bb.ly;

    return true;
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
        rect->umat = glGetUniformLocation(rect->shader.program, "umat");
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
        text->umat = glGetUniformLocation(text->shader.program, "umat");
        text->ucolor = glGetUniformLocation(text->shader.program, "ucolor");
        text->usampler = glGetUniformLocation(text->shader.program, "usampler");
        text->uchar = glGetUniformLocation(text->shader.program, "uchar");
        text->usize = glGetUniformLocation(text->shader.program, "usize");
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

static void draw_rect(sui_cmd cmd, sui_renderer *r, const float *matrix)
{
    struct sui_renderer_rect *rect = &r->rect;
    glUseProgram(rect->shader.program);
    glUniformMatrix3fv(rect->upos, 1, GL_TRUE, cmd.mat.data);
    glUniformMatrix4fv(rect->umat, 1, GL_TRUE, matrix);
    unsigned char *col = cmd.col;
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

static void sui_repeat_print(unsigned num, char c)
{
    for (unsigned i = 0; i < num; i++) {
        printf("%c", c);
    }
    printf("\n");
}

void sui_debug_print_atlas(sui_font *font)
{
    for (unsigned i = 0; i < font->length; i++) {
        printf("Entry for codepoint %u:\n", font->codepoints[i]);
        sui_repeat_print(font->width, '_');
        unsigned w,h;
        w = (font->metrics[i].width + 63) / 64;
        h = (font->metrics[i].height + 63) / 64;
        assert(h <= font->height);
        unsigned diff = 0; //font->height - h;
        sui_debug_print_image(font->data + font->width*font->height*i + font->width*diff, w, h, font->width);
        sui_repeat_print(font->width, '^');
    }
}

static void draw_text(sui_cmd cmd, sui_renderer *r, const float *matrix)
{
    struct sui_renderer_text *text = &r->text;
    sui_layout *l = cmd.data.text.layout;
    float em2ss = cmd.data.text.em;
    float fu2em = 1.0 / l->font->face->units_per_EM;
    float fu2ss = em2ss * fu2em;

    assert(l->align == SUI_ALIGN_TOPLEFT && "TODO: Other alignments");
    glUseProgram(text->shader.program);
    unsigned char *col = cmd.col;
    glUniformMatrix4fv(text->umat, 1, GL_TRUE, matrix);
    glUniform4f(text->ucolor, col[0] / 255.0, col[1] / 255.0, col[2] / 255.0, col[3] / 255.0);
    glUniform1i(text->usampler, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, l->font->tex);

    int pen_x = 0, pen_y = 0;
    for (unsigned i = 0; i < l->glyph_count; i++) {
        unsigned id = sui_font_getGlyph(l->font, l->glyph_info[i].codepoint);
        FT_Glyph_Metrics *metrics = &l->font->metrics[id];
        float p2ss = em2ss / (64.0*glyph_size);
        float x = (p2ss * pen_x) + (fu2ss * metrics->horiBearingX) + cmd.mat.data[2];
        float y = (p2ss * pen_y) + (fu2ss * metrics->horiBearingY) + cmd.mat.data[5];
        // size of the region to draw
        float w = fu2ss * metrics->width;
        float h = fu2ss * metrics->height;
        sui_mat3 mat = sui_size(x, y, w, -h);
        glUniformMatrix3fv(text->upos, 1, GL_TRUE, mat.data);
        glUniform1i(text->uchar, id);
        // size of the texture region to sample
        glUniform2f(text->usize,
                    (metrics->width+63) / 64 / (float)l->font->width,
                    (metrics->height+63) / 64 / (float)l->font->height);
        tgl_quad_draw_once(&r->vbo);
        pen_x += l->glyph_pos[i].x_advance;
        pen_y += l->glyph_pos[i].y_advance;
    }
}

void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, sui_cmd *cmds, size_t len, const float *matrix)
{
    (void)w, (void)h;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static const float ident[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    if (!matrix) {
        matrix = ident;
    }

    tgl_vao_bind(&r->vao);
    for (unsigned i = 0; i < len; i++) {
        switch (cmds[i].type) {
        case SUI_RECT:
            draw_rect(cmds[i], r, matrix);
            break;
        case SUI_TEXT:
            draw_text(cmds[i], r, matrix);
            break;
        }
    }
    glDisable(GL_BLEND);
}
