#include <stdio.h>
#include <cairo/cairo.h>
#include "../../src/sui.h"
#include "../../src/widget.h"
#include "../../src/widget/button.h"
#include "../../src/widget/rect.h"
#include "../../src/widget/image.h"
#include <GLFW/glfw3.h>

static sui_rect *rect;

void e(sui_button *self){
	sui_rect_set_color(rect, 1, 0, 0, 1);
}

void l(sui_button *self){
	sui_rect_set_color(rect, 0, 1, 0, 1);
}

int main(void)
{
	if (!glfwInit())
		return -1;

	GLFWwindow *window;
	sui_stage *stage;
	sui_button *button;
	//sui_image *image;
	node *layer1;

	window = glfwCreateWindow(550, 400, "Hello World", NULL, NULL);
	stage = sui_stage_new(550, 400);
	layer1 = sui_layer_new(stage);
	button = sui_button_new(5, 20, 100, 10);
	rect = sui_rect_new(5, 20, 100, 10);
	//image = sui_image_new(275, 200, 20, 100, "asdf.png");
	sui_button_leave(button, e);
	sui_button_up(button, l);
	sui_stage_add(stage, (sui_widget*)button, layer1);
	sui_stage_add(stage, (sui_widget*)rect, layer1);
	//sui_stage_add(stage, (sui_widget*)image, layer1);
	sui_rect_set_color(rect, 1, 0, 0, 1);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	sui_init(stage);
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		sui_run(stage, x, y, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	sui_terminate(stage);
	glfwTerminate();

	return 0;
}
