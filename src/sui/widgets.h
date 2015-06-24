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

static inline sui_mat3 sui_scale(sui_mat3 m, float w, float h)
{
    sui_mat3 mat = {{
            m.data[0]*w, m.data[1]*w, m.data[2]*w,
            m.data[3]*h, m.data[4]*h, m.data[5]*h,
            m.data[6]*1, m.data[7]*1, m.data[8]*1
        }};
    return mat;
}

static inline float sui_dot3(float a[3], float b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static inline sui_mat3 sui_mul(sui_mat3 a, sui_mat3 b)
{
    sui_mat3 res;
#define sui_idx(m, c,r) m.data[r*3+c]
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            float col[3] = { sui_idx(b, j,0), sui_idx(b, j,1), sui_idx(b, j,2) };
            sui_idx(res, j,i) = sui_dot3(&sui_idx(a, 0,i), col);
        }
    }
#undef sui_idx
    return res;
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
