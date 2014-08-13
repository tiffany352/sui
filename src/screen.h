#ifndef SCREEN_H
#define SCREEN_H

#include <cairo/cairo.h>

struct sui_state;

struct sui_state *sui_screen_update(struct sui_state *state);
struct sui_state *sui_screen_draw(struct sui_state *state, void(*func)(cairo_t *cr));
void sui_screen_clear(cairo_t *cr);

#endif
