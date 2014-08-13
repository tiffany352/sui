#include "keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>

struct sui_state {
	int width;
	int height;
	cairo_surface_t *surface;
	struct sui_keys *keys;
	struct sui_mouse *mouse;
	unsigned int texture_id[1];
};

struct sui_keys *sui_keys_init()
{
	struct sui_keys *keys;

	keys = malloc(sizeof(struct sui_keys));
	keys->down = NULL;
	keys->up = NULL;

	return keys;
}

struct sui_state *sui_key_down(struct sui_state *state, void(*down)(char))
{
	state->keys->down = down;
	return state;
}

struct sui_state *sui_key_up(struct sui_state *state, void(*up)(char))
{
	state->keys->up = up;
	return state;
}
