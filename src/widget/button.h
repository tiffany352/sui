#ifndef BUTTON_H
#define BUTTON_H

#include "../internal.h"
#include "../stage.h"
#include "../collision.h"

typedef struct sui_button {
    sui_stage *stage;
    void (*draw)(struct sui_button*, cairo_t*);
    void (*update)(struct sui_button*, sui_stage*);

    int x;
    int y;
    int width;
    int height;

    int over_;
    int pressed_;

    void (*enter)(struct sui_button*);
    void (*leave)(struct sui_button*);
    void (*down)(struct sui_button*);
    void (*up)(struct sui_button*);
} sui_button;

sui_button *sui_button_new(int x, int y, int width, int height);
void sui_button_set_position(sui_button *button, int x, int y);
void sui_button_enter(sui_button *button, void(*callback)(sui_button *self));
void sui_button_leave(sui_button *button, void(*callback)(sui_button *self));
void sui_button_down(sui_button *button, void(*callback)(sui_button *self));
void sui_button_up(sui_button *button, void(*callback)(sui_button *self));

#endif
