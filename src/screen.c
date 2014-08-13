#include "screen.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <cairo/cairo.h>
#include <stdio.h>

struct sui_state {
	int width;
	int height;
	cairo_surface_t *surface;
	struct sui_keys *keys;
	struct sui_mouse *mouse;
	unsigned int texture_id[1];
};

struct sui_state *sui_screen_update(struct sui_state *state)
{

	int width = state->width;
	int height = state->height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *(state->texture_id));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(0, 1);
	glVertex2f(0, height);
	glTexCoord2f(1, 1);
	glVertex2f(width, height);
	glTexCoord2f(1, 0);
	glVertex2f(width, 0);
	glEnd();

	return state;
}

struct sui_state *sui_screen_draw(struct sui_state *state, void(*func)(cairo_t *cr))
{
	cairo_surface_t *surface;
	cairo_t *cr;
	int width;
	int height;

	surface = state->surface;
	cr = cairo_create(surface);
	width = state->width;
	height = state->height;

	func(cr);

	glBindTexture(GL_TEXTURE_2D, state->texture_id[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
		cairo_image_surface_get_data(state->surface));

	cairo_destroy(cr);
	cairo_surface_flush(surface);

	return state;
}

void sui_screen_clear(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
}
