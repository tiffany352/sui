#ifndef WIDGET_H
#define WIDGET_H

#include "internal.h"
#include "stage.h"

typedef struct sui_widget {
	sui_stage *stage;
	void (*draw)(struct sui_widget*, cairo_t*);
	void (*update)(struct sui_widget*, sui_stage*);
} sui_widget;

sui_widget *sui_widget_new(void (*draw)(sui_widget*, cairo_t*));

#endif
