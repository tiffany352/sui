#include "rect.h"


static void draw(sui_rect *self, cairo_t *cr)
{
	cairo_set_source_rgba(cr, self->r, self->g, self->b, self->a);
	cairo_rectangle(cr, self->x, self->y, self->width, self->height);
	cairo_fill(cr);
}

sui_rect *sui_rect_new(int x, int y, int width, int height)
{
	sui_rect *rect;

	rect = malloc(sizeof(sui_rect));
	rect->stage = NULL;
	rect->draw = draw;
	rect->update = NULL;
	rect->x = x;
	rect->y = y;
	rect->width = width;
	rect->height = height;
	rect->r = 0;
	rect->g = 0;
	rect->b = 0;
	rect->a = 0;

	return rect;
}

void sui_rect_set_position(sui_rect *rect, int x, int y)
{
	rect->x = x;
	rect->y = y;
	rect->stage->dirty = 1;
}

void sui_rect_set_color(sui_rect *rect, float r, float g, float b, float a)
{
	rect->r = r;
	rect->g = g;
	rect->b = b;
	rect->a = a;
	rect->stage->dirty = 1;
}

void sui_rect_set_size(sui_rect *rect, int width, int height)
{
	rect->width = width;
	rect->height = height;
	rect->stage->dirty = 1;
}
