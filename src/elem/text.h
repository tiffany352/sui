#ifndef TEXT_H
#define TEXT_H

#include "../internal.h"
#include "../stage.h"

typedef struct sui_text {
	sui_stage *stage;
	void (*draw)(struct sui_text*, cairo_t*);
	void (*update)(struct sui_text*, sui_stage*);

	int x;
	int y;
	char *content;
} sui_text;

sui_text *sui_text_new(int x, int y, char *content);
void sui_text_set_position(sui_text *text, int x, int y);
void sui_text_set_color(sui_rect *rect, float r, float g, float b, float a);
void sui_text_set_text(sui_text *text, char *content);

#endif
