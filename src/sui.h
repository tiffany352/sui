#ifndef SUI_H
#define SUI_H

#include <cairo/cairo.h>
#include "keys.h"
#include "mouse.h"
#include "screen.h"

struct sui_state {
	int width;
	int height;
	cairo_surface_t *surface;
	struct sui_keys *keys;
	struct sui_mouse *mouse;
	unsigned int texture_id[1];
};

struct sui_state *sui_init(int width, int height);
struct sui_state *sui_run(struct sui_state *state);
struct sui_state *sui_terminate(struct sui_state *state);

#endif
