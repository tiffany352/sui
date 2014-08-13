#include "sui.h"
#include "mouse.h"
#include "collision.h"
#include "uthash/utlist.h"
#include "uthash/utarray.h"

struct sui_mouse *sui_mouse_init()
{
	struct sui_mouse *mouse;

	mouse = malloc(sizeof(struct sui_mouse));
	mouse->down_funcs = NULL;
	mouse->up_funcs = NULL;

	return mouse;
}

struct sui_state *sui_mouse_update(struct sui_state *state, int x, int y, int down)
{
	struct node *obj;
	struct _sui_region *region;
	struct sui_mouse *mouse;

	mouse = state->mouse;

	if (down) {
		DL_FOREACH(mouse->down_funcs, obj) {
			region = obj->val;
			if (collides(x, y, region->x, region->y, region->width, region->height))
				region->func();
		}
	}
	else {
		DL_FOREACH(mouse->up_funcs, obj) {
			region = obj->val;
			if (collides(x, y, region->x, region->y, region->width, region->height))
				region->func();
		}
	}

	return state;
}

struct sui_state *sui_mouse_down(struct sui_state *state, int x, int y, int width, int height, void(*func)())
{
	struct _sui_region *region;
	struct sui_mouse *mouse;
	struct node *obj;

	region = malloc(sizeof(struct _sui_region));
	region->x = x;
	region->y = y;
	region->width = width;
	region->height = height;
	region->func = func;

	mouse = state->mouse;
	obj = malloc(sizeof(struct node));
	obj->val = region;
	DL_APPEND(mouse->down_funcs, obj);

	return state;
}

struct sui_state *sui_mouse_up(struct sui_state *state, int x, int y, int width, int height, void(*func)())
{
	struct _sui_region *region;
	struct sui_mouse *mouse;
	struct node *obj;

	region = malloc(sizeof(struct _sui_region));
	region->x = x;
	region->y = y;
	region->width = width;
	region->height = height;
	region->func = func;

	mouse = state->mouse;
	obj = malloc(sizeof(struct node));
	obj->val = region;
	DL_APPEND(mouse->up_funcs, obj);

	return state;
}
