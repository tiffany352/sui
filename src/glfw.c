#include "glfw.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "sui.h"

static GLFWwindow *window;
static struct sui_state *state;
static struct sui_keys *keys;

void _sui_glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && keys->down)
		keys->down((char)key);

	if (action == GLFW_REPEAT && keys->up)
		keys->up((char)key);
}

struct sui_state *sui_glfw_init(int width, int height, char *title)
{
	if (!glfwInit())
		return NULL;

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		glfwTerminate();
		return NULL;
	}

	state = sui_init(width, height);
	keys = state->keys;
	glfwSetKeyCallback(window, _sui_glfw_keycallback);

	return state;
}

struct sui_state *sui_glfw_loop()
{
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		sui_screen_update(state);
		sui_mouse_update(state, x, y, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return state;
}

struct sui_state *sui_glfw_terminate()
{
	sui_terminate(state);
	glfwTerminate();

	return state;
}
