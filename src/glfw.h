#ifndef GLFW_H
#define GLFW_H

struct sui_state *sui_glfw_init(int width, int height, char *title);
struct sui_state *sui_glfw_loop();
struct sui_state *sui_glfw_terminate();

#endif
