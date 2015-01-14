#ifndef SUI_WIDGETS_H
#define SUI_WIDGETS_H

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

#define sui_col(r,g,b,a) (uint8_t[]){r,g,b,a}

enum sui_cmd_type {
    SUI_RECT,
    SUI_TEXT,
};

typedef struct sui_wcmd {
    enum sui_cmd_type type;
    unsigned char col[4];
    union {
        int dummy;
        struct {
            sui_textfmt fmt;
            const char *text;
        } text;
    } data;
} sui_wcmd;

static inline sui_wcmd sui_wrect(unsigned char col[4])
{
    sui_wcmd cmd = {
        SUI_RECT,
        {col[0], col[1], col[2], col[3]},
        {0}
    };
    return cmd;
}

static inline sui_wcmd sui_wtext(unsigned char col[4], sui_textfmt fmt, const char *text)
{
    sui_wcmd cmd = {
        SUI_TEXT,
        {col[0], col[1], col[2], col[3]},
        {0}
    };
    cmd.data.text.fmt = fmt;
    cmd.data.text.text = text;
    return cmd;
}

#endif
