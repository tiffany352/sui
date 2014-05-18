#ifndef RECT_H
#define RECT_H

#include "../internal.h"
#include "../stage.h"
#include "../collision.h"

typedef struct sui_rect {
	sui_stage *stage;
	void (*draw)(struct sui_rect*, cairo_t*);
	void (*update)(struct sui_rect*, sui_stage*);

	int x;
	int y;
	int width;
	int height;
	float r;
	float g;
	float b;
	float a;
} sui_rect;

sui_rect *sui_rect_new(int x, int y, int width, int height);
void sui_rect_set_position(sui_rect *rect, int x, int y);
void sui_rect_set_color(sui_rect *rect, float r, float g, float b, float a);
void sui_rect_set_size(sui_rect *rect, int width, int height);

#endif
