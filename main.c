#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cairo/cairo.h>
#include "src/sui.h"
#include "src/widget.h"
#include "src/keys.h"
#include "src/widget/button.h"
#include "src/widget/rect.h"
#include "src/widget/image.h"

static sui_rect *rect;

void e(sui_button *self){
	sui_rect_set_color(rect, 1, 0, 0, 1);
}

void l(sui_button *self){
	sui_rect_set_color(rect, 0, 1, 0, 1);
}

void m(int x){
	sui_rect_set_color(rect, 0, 1, 0, 1);
}

int main(void)
{
	if (!sui_init())
		return -1;

	GLFWwindow *window;
	sui_stage *stage;
	sui_keys *keys;
	sui_button *button;
	//sui_image *image;
	node *layer1;

	window = glfwCreateWindow(550, 400, "Hello World", NULL, NULL);
	keys = sui_keys_new(window);
	stage = sui_stage_new(window);
	layer1 = sui_layer_new(stage);
	button = sui_button_new(5, 20, 100, 10);
	rect = sui_rect_new(5, 20, 100, 10);
	//image = sui_image_new(275, 200, 20, 100, "asdf.png");
	sui_keys_register(keys, GLFW_KEY_A, m);
	sui_button_leave(button, e);
	sui_button_up(button, l);
	sui_stage_add(stage, (sui_widget*)button, layer1);
	sui_stage_add(stage, (sui_widget*)rect, layer1);
	//sui_stage_add(stage, (sui_widget*)image, layer1);
	sui_rect_set_color(rect, 1, 0, 0, 1);

	if (!window) {
		sui_terminate();
		return -1;
	}

	sui_run(stage, keys);
	sui_terminate();

	return 0;
}
