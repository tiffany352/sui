#include "text.h"

static void draw(sui_text *self, cairo_t *cr)
{
	cairo_set_source_rgba(cr, self->r, self->g, self->b, self->a);
	cairo_move_to(cr, self->x, self->y);
	cairo_show_text(cr, self->content);
}

sui_text *sui_text_new(int x, int y, char *content)
{
	sui_text *text;

	text = malloc(sizeof(sui_text));
	text->stage = NULL;
	text->draw = draw;
	text->update = NULL;
	text->x = x;
	text->y = y;
	text->r = 0;
	text->g = 0;
	text->b = 0;
	text->a = 0;
	text->content = content;

	return text;
}

void sui_text_set_position(sui_text *text, int x, int y)
{
	text->x = x;
	text->y = y;
	text->stage->dirty = 1;
}

void sui_text_set_color(sui_text *text, float r, float g, float b, float a)
{
	text->r = r;
	text->g = g;
	text->b = b;
	text->a = a;
	text->stage->dirty = 1;
}

void sui_text_set_content(sui_text *text, char *content)
{
	text->content = content;
	text->stage->dirty = 1;
}
