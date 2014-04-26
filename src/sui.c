#include "sui.h"

void sui_init(sui_stage *stage)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glGenTextures(1, stage->texture_id);
}

void sui_run(sui_stage *stage, double x, double y, int down)
{
	int width = stage->width;
	int height = stage->height;
	stage->x = x;
	stage->y = y;
	stage->down = down;

	sui_stage_update(stage);
	if (stage->dirty) {
		sui_stage_draw(stage);
		glBindTexture(GL_TEXTURE_2D, stage->texture_id[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
				cairo_image_surface_get_data(stage->surface));
	}

	// render texture
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *(stage->texture_id));

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
}

void sui_terminate(sui_stage *stage)
{
	glDeleteTextures(1, &(stage->texture_id[0]));
}
