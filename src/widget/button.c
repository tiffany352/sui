#include "button.h"

static void update(sui_button *self, sui_stage *stage)
{
    double x, y;
    GLFWwindow *window;

    window = stage->window;
    glfwGetCursorPos(window, &x, &y);

    if (collides(x, y, self->x, self->y, self->width, self->height)) {
        if (!self->over_ && self->enter) {
            self->enter(self);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            if (!self->pressed_ && self->down) {
                self->down(self);
            }

            self->pressed_ = 1;
        } else {
            if (self->pressed_ && self->up) {
                self->up(self);
            }

            self->pressed_ = 0;
        }

        self->over_ = 1;
    } else {
        if (self->over_ && self->leave) {
            self->leave(self);
        }

        self->over_ = 0;
    }
}

sui_button *sui_button_new(int x, int y, int width, int height)
{
    sui_button *button;

    button = malloc(sizeof(sui_button));
    button->stage = NULL;
    button->draw = NULL;
    button->update = update;
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->over_ = 0;
    button->pressed_ = 0;
    button->enter = NULL;
    button->leave = NULL;
    button->down = NULL;
    button->up = NULL;

    return button;
}

void sui_button_set_position(sui_button *button, int x, int y)
{
    button->x = x;
    button->y = y;
}

void sui_button_enter(sui_button *button, void(*callback)(sui_button *self)){
    button->enter = callback;
}

void sui_button_leave(sui_button *button, void(*callback)(sui_button *self)){
    button->leave = callback;
}

void sui_button_down(sui_button *button, void(*callback)(sui_button *self)){
    button->down = callback;
}

void sui_button_up(sui_button *button, void(*callback)(sui_button *self)){
    button->up = callback;
}
