#include "stage.h"
#include "widget.h"

sui_stage *sui_stage_new(int width, int height)
{
	sui_stage *stage;

	stage = malloc(sizeof(sui_stage));
	stage->width = width;
	stage->height = height;
	stage->x = 0;
	stage->y = 0;
	stage->down = 0;
	stage->layers = NULL;
	stage->surface =
		cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
	stage->dirty = 1;

	return stage;
}

node *sui_layer_new(sui_stage *stage)
{
	node *objs;
	node *layer;
	objs = NULL;

	layer = malloc(sizeof(node));
	layer->val = objs;
	DL_APPEND(stage->layers, layer);

	return layer;
}

void sui_stage_add(sui_stage *stage, sui_widget *widget, node *layer)
{
	node *obj;
	node *objs = layer->val;

	widget->stage = stage;
	obj = malloc(sizeof(node));
	obj->val = widget;
	DL_APPEND(objs, obj);
	layer->val = objs;

	stage->dirty = 1;
}

void sui_stage_draw(sui_stage *stage)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	node *obj;
	node *layer;
	int i;

	surface = stage->surface;
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	DL_FOREACH(stage->layers, layer) {
		DL_FOREACH(layer->val, obj) {
			sui_widget *widget = obj->val;
			if (widget->draw)
				widget->draw(widget, cr);
		}
	}

	cairo_destroy(cr);
	cairo_surface_flush(surface);

	stage->dirty = 0;
}

void sui_stage_update(sui_stage *stage)
{
	int i;
	node *obj;
	node *layer;

	DL_FOREACH(stage->layers, layer) {
		DL_FOREACH(layer->val, obj) {
			sui_widget *widget = obj->val;
			if (widget->update)
				widget->update(widget, stage);
		}
	}
}
