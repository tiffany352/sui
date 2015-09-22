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

#define sui_col(r,g,b,a) (uint8_t[]){r,g,b,a}

enum sui_cmd_type {
    SUI_RECT,
    SUI_TEXT,
};

typedef struct sui_point {
    int x, y;
} sui_point;

#define sui_mkpoint(x,y) ((sui_point) { x, y })

struct sui_layout;

typedef struct sui_cmd {
    enum sui_cmd_type type;
    unsigned char col[4];
    sui_point position;
    union {
        int dummy;
        sui_point rect_size;
        struct sui_layout *text;
    } data;
} sui_cmd;

static inline sui_cmd sui_rect(unsigned char col[4], sui_point position, sui_point size)
{
    sui_cmd cmd = {
        SUI_RECT,
        {col[0], col[1], col[2], col[3]},
        position,
        {0}
    };
    cmd.data.rect_size = size;
    return cmd;
}

static inline sui_cmd sui_text(unsigned char col[4], sui_point position, struct sui_layout *layout)
{
    sui_cmd cmd = {
        SUI_TEXT,
        {col[0], col[1], col[2], col[3]},
        position,
        {0}
    };
    cmd.data.text = layout;
    return cmd;
}

#endif
