#include "sui.h"
#include "screen.h"
#include "keys.h"
#include "mouse.h"
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <cairo/cairo.h>

struct sui_state *sui_init(int width, int height)
{
	struct sui_state *state;

	state = malloc(sizeof(struct sui_state));
	state->width = width;
	state->height = height;
	state->surface =
		cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
	state->keys = sui_keys_init();
	state->mouse = sui_mouse_init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glGenTextures(1, state->texture_id);

	return state;
}

struct sui_state *sui_terminate(struct sui_state *state)
{
	glDeleteTextures(1, &(state->texture_id[0]));

	return state;
}
