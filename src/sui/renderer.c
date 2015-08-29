#include "renderer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <hb-icu.h>
#include <fontconfig/fcfreetype.h>

#include "sui/widgets.h"

enum sui_arrattribs {
    SUI_RECT_POS
};

bool sui_layout_init(sui_layout *layout, sui_font *font, const sui_layout_format *fmt,
                     const char *text, size_t length, char **error)
{
    static const int text_dir_table[] = {
        HB_DIRECTION_LTR,
        HB_DIRECTION_RTL,
        HB_DIRECTION_TTB,
        HB_DIRECTION_BTT
    };

    memset(layout, 0, sizeof(sui_layout));
    layout->fmt = *fmt;
    layout->font = font;
    FT_Error fterr;
    if ((fterr = FT_Set_Pixel_Sizes(font->face, 0, fmt->size))) {
        *error = sui_aprintf("FT_Set_Char_Size: %i\n", fterr);
        return false;
    }
    layout->buffer = hb_buffer_create();
    hb_buffer_set_unicode_funcs(layout->buffer, hb_icu_get_unicode_funcs());
    hb_buffer_set_direction(layout->buffer, text_dir_table[fmt->dir]);
    hb_script_t script = hb_script_from_string(fmt->script, -1);
    if (script == HB_SCRIPT_INVALID || script == HB_SCRIPT_UNKNOWN) {
        *error = sui_aprintf("Invalid script: %s\n", fmt->script);
        return false;
    }
    hb_buffer_set_script(layout->buffer, script);
    hb_language_t lang = hb_language_from_string(fmt->lang, -1);
    if (lang == HB_LANGUAGE_INVALID) {
        *error = sui_aprintf("Invalid language: %s\n", fmt->lang);
        return false;
    }
    hb_buffer_set_language(layout->buffer, lang);
    hb_buffer_add_utf8(layout->buffer, text, length, 0, length);
    hb_shape(font->hb_font, layout->buffer, NULL, 0);

    layout->infos = hb_buffer_get_glyph_infos(layout->buffer, &layout->count);
    layout->positions = hb_buffer_get_glyph_positions(layout->buffer, &layout->count);

    return true;
}

void sui_layout_free(sui_layout *layout)
{
    hb_buffer_destroy(layout->buffer);
}

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

const char *sui_result_name(FcResult res)
{
    switch (res) {
    case FcResultMatch:        return "FcResultMatch";
    case FcResultNoMatch:      return "FcResultNoMatch";
    case FcResultTypeMismatch: return "FcResultTypeMismatch";
    case FcResultNoId:         return "FcResultNoId";
    case FcResultOutOfMemory:  return "FcResultOutOfMemory";
    default:                   return "Unknown result";
    }
}

bool sui_font_fromfc(sui_font *font, sui_renderer *r, char **error, FcPattern *pattern)
{
    FcResult res;
    FcConfig *config = FcConfigGetCurrent();
    if (!FcConfigSubstitute(config, pattern, FcMatchFont)) {
        *error = sui_aprintf("FcConfigSubstitute: Allocation failure");
        return false;
    }
    FcDefaultSubstitute(pattern);
    FcPattern *match = FcFontMatch(config, pattern, &res);
    if (res != FcResultMatch) {
        *error = sui_aprintf("FcFontMatch: %s", sui_result_name(res));
        return false;
    }
    FcChar8 *file;
    int index;
    res = FcPatternGetString(match, FC_FILE, 0, &file);
    if (res != FcResultMatch) {
        *error = sui_aprintf("FcPatternGetString FC_FILE: %s", sui_result_name(res));
        return false;
    }
    res = FcPatternGetInteger(match, FC_INDEX, 0, &index);
    if (res != FcResultMatch) {
        *error = sui_aprintf("FcPatternGetInteger FC_INDEX: %s", sui_result_name(res));
        return false;
    }
    FT_Error fterr;
    if ((fterr = FT_New_Face(r->text.library, (const char*)file, index, &font->face))) {
        *error = sui_aprintf("FT_New_Face: Error code %i", fterr);
        return false;
    }
    return font_fromfont(font, r, error, font->face);
}

bool sui_font_fromfamily(sui_font *font, sui_renderer *r, char **error, const char *family)
{
    return sui_font_fromfc(font, r, error,
                           FcPatternBuild(0, FC_FAMILY, FcTypeString, family, (char*)0));
}

void sui_font_free(sui_font *font)
{
    hb_font_destroy(font->hb_font);
    hb_face_destroy(font->hb_face);
    FT_Done_Face(font->face);
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

void sui_renderer_free(sui_renderer *r)
{
    tgl_vao_free(&r->vao);
    tgl_quad_free(&r->vbo);
    //tgl_shader_free(&r->rect.shader);
    //tgl_shader_free(&r->text.shader);
    FT_Done_FreeType(r->text.library);
    for (unsigned i = 0; i < r->text.glyphs_size; i++) {
        glDeleteTextures(1, &r->text.glyphs[i].tex);
    }
    free(r->text.glyphs);
}

sui_glyph *sui_renderer_get_glyph(sui_renderer *r, uint32_t codepoint, unsigned size, sui_font *font)
{
    // perform a binary search
    int min = 0, max = r->text.glyphs_size - 1;
    while (min <= max) {
        int mid = min + (max-min)/2;
        sui_glyph *glyph = &r->text.glyphs[mid];
        if (glyph->codepoint == codepoint && glyph->size == size && glyph->font == font) {
            return glyph;
        }
        if (glyph->codepoint < codepoint ||
            glyph->size < size ||
            (uintptr_t)glyph->font < (uintptr_t)font) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    assert(min == max + 1);
    if (r->text.glyphs_size == r->text.glyphs_capacity) {
        size_t new_capacity = r->text.glyphs_capacity? r->text.glyphs_capacity * 2 : 64;
        sui_glyph *glyphs = calloc(new_capacity, sizeof(sui_glyph));
        memcpy(glyphs, r->text.glyphs, r->text.glyphs_size * sizeof(sui_glyph));
        r->text.glyphs_capacity = new_capacity;
        free(r->text.glyphs);
        r->text.glyphs = glyphs;
    }
    memmove(r->text.glyphs + min + 1,
            r->text.glyphs + min,
            (r->text.glyphs_size - min) * sizeof(sui_glyph));
    r->text.glyphs_size++;
    sui_glyph *glyph = &r->text.glyphs[min];
    glyph->codepoint = codepoint;
    glyph->size = size;
    glyph->font = font;

    FT_Error fterr;
    FT_Face face = font->face;
    if ((fterr = FT_Set_Pixel_Sizes(face, 0, size))) {
        printf("FT_Set_Char_Size: %i\n", fterr);
        abort();
    }
    if ((fterr = FT_Load_Glyph(face, codepoint, FT_LOAD_DEFAULT))) {
        printf("FT_Load_Glyph: %i\n", fterr);
        abort();
    }
    FT_GlyphSlotRec *ftglyph = face->glyph;
    if (ftglyph->format != FT_GLYPH_FORMAT_BITMAP && (fterr = FT_Render_Glyph(ftglyph, FT_RENDER_MODE_NORMAL))) {
        printf("FT_Render_Glyph: %i\n", fterr);
        abort();
    }
    glyph->left = ftglyph->bitmap_left;
    glyph->top = ftglyph->bitmap_top;
    FT_Bitmap *bm = &ftglyph->bitmap;
    glyph->width = bm->width;
    glyph->rows = bm->rows;
    glGenTextures(1, &glyph->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glyph->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bm->width, bm->rows, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    for (unsigned i = 0; i < bm->rows; i++) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, bm->width, 1, GL_RED, GL_UNSIGNED_BYTE,
                        bm->buffer + bm->pitch * i);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return glyph;
}

static void draw_rect(sui_cmd cmd, unsigned w, unsigned h, sui_renderer *r)
{
    struct sui_renderer_rect *rect = &r->rect;
    glUseProgram(rect->shader.program);
    float ux = cmd.position.x / (float)w;
    float uy = 1.0 - cmd.position.y / (float)h;
    float uw = cmd.data.rect_size.x / (float)w;
    float uh = cmd.data.rect_size.y / -(float)h;
    glUniform4f(rect->upos, ux, uy, uw, uh);
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

static void draw_text(sui_cmd cmd, unsigned w, unsigned h, sui_renderer *r)
{
    struct sui_renderer_text *text = &r->text;
    sui_layout *layout = cmd.data.text;
    FT_Face face = layout->font->face;
    FT_Error fterr;
    if ((fterr = FT_Set_Pixel_Sizes(face, 0, layout->fmt.size))) {
        printf("FT_Set_Char_Size: %i\n", fterr);
        abort();
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glUseProgram(text->shader.program);
    unsigned char *col = cmd.col;
    glUniform4f(text->ucolor, col[0] / 255.0, col[1] / 255.0, col[2] / 255.0, col[3] / 255.0);
    glUniform1i(text->usampler, 0);
    int pen_x = 0, pen_y = 0;
    for (unsigned i = 0; i < layout->count; i++) {
        // not a unicode codepoint.
        unsigned codepoint = layout->infos[i].codepoint;
        unsigned top = layout->fmt.size;
        sui_glyph *glyph = sui_renderer_get_glyph(r, codepoint, layout->fmt.size, layout->font);
        // prevents wrapping to the left side
        if (i == 0) {
            pen_x = glyph->left * 64;
        }
        float ux = (cmd.position.x +
                    pen_x / 64 -
                    layout->positions[i].x_offset +
                    glyph->left) / (float)w;
        float uy = 1.0 - (cmd.position.y +
                          pen_y / 64 -
                          layout->positions[i].y_offset -
                          glyph->top +
                          top) / (float)h;
        float uw = glyph->width / (float)w;
        float uh = glyph->rows / -(float)h;
        glUniform4f(text->upos, ux, uy, uw, uh);
        glBindTexture(GL_TEXTURE_2D, glyph->tex);
        tgl_quad_draw_once(&r->vbo);
        pen_x += layout->positions[i].x_advance;
        pen_y += layout->positions[i].y_advance;
    }
}

void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, sui_cmd *cmds, size_t len)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tgl_vao_bind(&r->vao);
    for (unsigned i = 0; i < len; i++) {
        switch (cmds[i].type) {
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
