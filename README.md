# SUI
## Summary
The Sane User Interface Library.

A simple UI building library written in C.  It has only a few dependencies, GLFW and cairo.

## API
### void sui_init(sui_stage *stage)
### node *sui_layer_new(sui_stage *stage)
### void sui_run(sui_stage *stage, double x, double y, int down)
### void sui_stage_add(sui_stage *stage, sui_widget *widget, node *layer)
### void sui_stage_draw(sui_stage *stage)
### sui_stage *sui_stage_new(int width, int height)
### void sui_stage_update(sui_stage *stage)
### void sui_terminate(sui_stage *stage)
### sui_widget *sui_widget_new(void (*draw)(sui_widget*, cairo_t*))
