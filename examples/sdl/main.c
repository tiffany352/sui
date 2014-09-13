#include <stdio.h>
#include <SDL.h>

#include "tgl/tgl.h"
#include "widgets/widgets.h"
#include "render/renderer.h"

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("SDL Example", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event ev;
    sui_renderer r[1];
    char *error;

    if (!sui_renderer_init(r, &error)) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    while (1) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                goto quit;
            }
        }
        glClearColor(0.4, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sui_cmd buf[] = {
            sui_rect(sui_size(0,0, 60,10), sui_col(255,100,125, 255)),
            sui_rect(sui_size(780,550, 20, 50), sui_col(255,255,255, 255)),
            sui_rect(sui_size(100,300, 200,40), sui_col(25,190,50, 255))
        };

        sui_renderer_draw(r, 800, 600, buf, sizeof(buf)/sizeof(sui_cmd));

        SDL_GL_SwapWindow(window);
    }
 quit:
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
