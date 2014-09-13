#ifndef SUI_WIDGETS_H
#define SUI_WIDGETS_H

typedef struct sui_aabb {
    unsigned lx,ly, hx,hy;
} sui_aabb;

#define sui_size(x,y,w,h) (sui_aabb){x,y,x+w,y+h}
#define sui_col(r,g,b,a) (uint8_t[]){r,g,b,a}

typedef struct sui_cmd {
    enum {
        SUI_RECT,
        SUI_PUSH,
        SUI_POP,
    } type;
    sui_aabb aabb;
    unsigned char col[4];
} sui_cmd;

typedef struct sui_button {
    sui_aabb aabb;
    unsigned char col[4];
} sui_button;

sui_cmd sui_rect(sui_aabb aabb, unsigned char col[4]);

#endif
