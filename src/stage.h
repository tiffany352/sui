#ifndef STAGE_H
#define STAGE_H

#include "internal.h"

struct sui_widget;

typedef node sui_layer;

typedef struct sui_stage {
    sui_layer *layers;
    cairo_surface_t *surface;
    GLFWwindow *window;
    int dirty;
} sui_stage;


sui_layer *sui_layer_new();
sui_stage *sui_stage_new(GLFWwindow *window);
void sui_stage_add(sui_stage *stage, struct sui_widget *widget, node *layer);
void sui_stage_draw(sui_stage *stage);
void sui_stage_update(sui_stage *stage);

#endif
