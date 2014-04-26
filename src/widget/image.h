#ifndef IMAGE_H
#define IMAGE_H

#include "../internal.h"
#include "../stage.h"
#include "../collision.h"

typedef struct sui_image {
	sui_stage *stage;
	void (*draw)(struct sui_image*, cairo_t*);
	void (*update)(struct sui_image*, sui_stage*);

	int x;
	int y;
	int width;
	int height;
	char *source;
} sui_image;

sui_image *sui_image_new(int x, int y, int width, int height, char *source);
void sui_image_set_position(sui_image *image, int x, int y);
void sui_image_set_size(sui_image *image, int width, int height);

#endif
