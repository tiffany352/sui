#include <stdio.h>
#include "../../src/sui.h"
#include "../../src/elem.h"
#include "../../src/elem/button.h"
#include "../../src/elem/rect.h"
#include "../../src/elem/image.h"
#include <SDL.h>

static sui_rect *rect;

void e(sui_button *self){
	sui_rect_set_color(rect, 1, 0, 0, 1);
}

void l(sui_button *self){
	sui_rect_set_color(rect, 0, 1, 0, 1);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_Event windowEvent;

	sui_stage *stage;
	sui_button *button;
	//sui_image *image;
	node *layer1;

	stage = sui_stage_new(550, 400);
	layer1 = sui_layer_new(stage);
	button = sui_button_new(5, 20, 100, 10);
	rect = sui_rect_new(5, 20, 100, 10);
	//image = sui_image_new(275, 200, 20, 100, "asdf.png");
	sui_button_leave(button, e);
	sui_button_up(button, l);
	sui_stage_add(stage, (sui_elem*)button, layer1);
	sui_stage_add(stage, (sui_elem*)rect, layer1);
	//sui_stage_add(stage, (sui_elem*)image, layer1);
	sui_rect_set_color(rect, 1, 0, 0, 1);

	sui_init(stage);
	while (1)
	{
		int x;
		int y;
		SDL_PumpEvents();
		SDL_GetMouseState(&x, &y);
		sui_run(stage, (double) x, (double) y, SDL_BUTTON(1));
		SDL_GL_SwapWindow(window);

		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_KEYUP &&
					windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

	}
	sui_terminate(stage);
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

