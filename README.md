# SUI
## Summary
The Sane User Interface Library.

A simple UI building library written in C.  It has only a few dependencies, GLFW and cairo.

## API
### int sui_init()

### void sui_run(sui_stage *stage)

### void sui_terminate()

### void sui_stage_add(sui_stage *stage, void *widget, int layer_id){

### void sui_stage_draw(sui_stage *stage)

### sui_stage *sui_stage_new(GLFWwindow *window, int num_layers)

### void sui_stage_update(sui_stage *stage)

### sui_widget *sui_widget_new(void (*draw)(cairo_t*))}
