#include "sui.h"

int sui_init()
{
    return glfwInit();
}

void sui_run(sui_stage *stage, sui_keys *keys)
{
    int width;
    int height;
    unsigned int texture_id[1];
    GLFWwindow *window;
    cairo_surface_t *surf;
    cairo_t *cr;

    window = stage->window;
    surf = stage->surface;
    glfwGetWindowSize(window, &width, &height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glGenTextures(1, texture_id);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        sui_stage_update(stage);
        sui_keys_update(keys);
        if (stage->dirty) {
            sui_stage_draw(stage);
            glBindTexture(GL_TEXTURE_2D, texture_id[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                    width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                    cairo_image_surface_get_data(surf));
        }

        // render texture
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(0, 0);
            glTexCoord2f(0, 1);
            glVertex2f(0, height);
            glTexCoord2f(1, 1);
            glVertex2f(width, height);
            glTexCoord2f(1, 0);
            glVertex2f(width, 0);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture_id[0]);
}

void sui_terminate()
{
    glfwTerminate();
}
