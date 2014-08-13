#include <stdio.h>
#include <cairo/cairo.h>
#include "../../src/sui.h"
#include "../../src/glfw.h"

static struct sui_state *state;

// draws
void paint_red(cairo_t *cr)
{
	cairo_set_source_rgba(cr, 1, 0, 0, 1);
	cairo_rectangle(cr, 0, 0, 550, 400);
	cairo_fill(cr);
}

void paint_black(cairo_t *cr)
{
	cairo_set_source_rgba(cr, 0, 0, 1, 1);
	cairo_rectangle(cr, 0, 0, 550, 400);
	cairo_fill(cr);
}

void call_paint_red()
{
	sui_screen_draw(state, paint_red);
}

void call_paint_black()
{
	sui_screen_draw(state, paint_black);
}

int main(void)
{
	// init
	state = sui_glfw_init(550, 400, "sui");
	if (!state)
		return -1;

	// events
	sui_mouse_down(state, 0, 0, 550, 400, call_paint_red);
	//sui_mouse_up(state, 0, 0, 550, 400, call_paint_black);

	//sui_key_down(state, call_paint_red);
	//sui_key_up(state, call_paint_black);

//	sui_screen_draw(state, sui_screen_clear);
//

	call_paint_black();

	// loop
	sui_glfw_loop();

	// terminate
	sui_glfw_terminate();

	return 0;
}
