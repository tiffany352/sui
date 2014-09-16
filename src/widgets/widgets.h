#ifndef SUI_WIDGETS_H
#define SUI_WIDGETS_H

typedef struct sui_aabb {
    unsigned lx,ly, hx,hy;
} sui_aabb;

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
    struct sui_font *font;
    const char *lang, *script;
} sui_textfmt;

#define sui_size(x,y,w,h) (sui_aabb){x,y,x+w,y+h}
#define sui_col(r,g,b,a) (uint8_t[]){r,g,b,a}

typedef struct sui_cmd {
    enum {
        SUI_RECT,
        SUI_TEXT,
    } type;
    sui_aabb aabb;
    unsigned char col[4];
    union {
        struct {
            sui_textfmt fmt;
            const char *text;
        } text;
        int dummy;
    } data;
} sui_cmd;

typedef struct sui_button {
    sui_aabb aabb;
    unsigned char col[4];
} sui_button;

static inline sui_cmd sui_rect(sui_aabb aabb, unsigned char col[4])
{
    sui_cmd cmd = {
        SUI_RECT,
        aabb,
        {col[0], col[1], col[2], col[3]},
        {.dummy = 0}
    };
    return cmd;
}

static inline sui_cmd sui_text(sui_aabb aabb, unsigned char col[4], sui_textfmt fmt, const char *text)
{
    sui_cmd cmd = {
        SUI_TEXT,
        aabb,
        {col[0], col[1], col[2], col[3]},
        {.text = {
                fmt,
                text }}
    };
    return cmd;
}

#endif
