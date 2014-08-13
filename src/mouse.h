#ifndef MOUSE_H
#define MOUSE_H

struct sui_state;

struct node {
	void *val;
	struct node *next, *prev;
};

struct sui_mouse {
	struct node *down_funcs;
	struct node *up_funcs;
};

struct _sui_region {
	int x;
	int y;
	int width;
	int height;
	void(*func)();
};

struct sui_mouse *sui_mouse_init();
struct sui_state *sui_mouse_update(struct sui_state *state, int x, int y, int down);
struct sui_state *sui_mouse_down(struct sui_state *state, int x, int y, int width, int height, void(*func)());
struct sui_state *sui_mouse_up(struct sui_state *state, int x, int y, int width, int height, void(*func)());

#endif
