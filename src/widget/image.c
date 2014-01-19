#include "image.h"


static void draw(sui_image *self, cairo_t *cr)
{
    cairo_surface_t *image = cairo_image_surface_create_from_png(self->source);
    float width_ratio =
        (float) self->width / cairo_image_surface_get_width(image);
    float height_ratio =
        (float) self->height / cairo_image_surface_get_height(image);

    cairo_scale(cr, width_ratio, height_ratio);
    cairo_set_source_surface(cr, image, self->x / width_ratio,
            self->y / height_ratio);
    cairo_paint(cr);
    cairo_surface_destroy(image);
}

sui_image *sui_image_new(int x, int y, int width, int height, char *source)
{
    sui_image *image;

    image = malloc(sizeof(sui_image));
    image->stage = NULL;
    image->draw = draw;
    image->update = NULL;
    image->x = x;
    image->y = y;
    image->width = width;
    image->height = height;
    image->source = source;

    return image;
}

void sui_image_set_position(sui_image *image, int x, int y)
{
    image->x = x;
    image->y = y;
    image->stage->dirty = 1;
}

void sui_image_set_size(sui_image *image, int width, int height)
{
    image->width = width;
    image->height = height;
    image->stage->dirty = 1;
}
