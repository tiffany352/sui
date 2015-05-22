#ifndef SUI_WIDGETS_H
#define SUI_WIDGETS_H

typedef struct sui_mat3 {
    float data[9];
} sui_mat3;

#define sui_col(r,g,b,a) (uint8_t[]){r,g,b,a}

enum sui_cmd_type {
    SUI_RECT,
    SUI_TEXT,
};

typedef struct sui_cmd {
    enum sui_cmd_type type;
    sui_mat3 mat;
    unsigned char col[4];
    union {
        int dummy;
        struct {
            struct sui_layout *layout;
            float em;
        } text;
    } data;
} sui_cmd;

static inline sui_mat3 sui_size(float x, float y, float w, float h)
{
    sui_mat3 mat = {{
            w, 0, x,
            0, h, y,
            0, 0, 1
        }};
    return mat;
}

static inline sui_mat3 sui_translate(sui_mat3 m, float x, float y)
{
    sui_mat3 mat = {{
            m.data[0], m.data[1], m.data[2] + x,
            m.data[3], m.data[4], m.data[5] + y,
            m.data[6], m.data[7], m.data[8]
        }};
    return mat;
}

static inline sui_cmd sui_rect(sui_mat3 mat, unsigned char col[4])
{
    sui_cmd cmd = {
        SUI_RECT,
        mat,
        {col[0], col[1], col[2], col[3]},
        {0}
    };
    return cmd;
}

static inline sui_cmd sui_text(sui_mat3 mat, unsigned char col[4], struct sui_layout *layout, float em)
{
    sui_cmd cmd = {
        SUI_TEXT,
        mat,
        {col[0], col[1], col[2], col[3]},
        {0}
    };
    cmd.data.text.layout = layout;
    cmd.data.text.em = em;
    return cmd;
}

#endif
