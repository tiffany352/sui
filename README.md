# SUI
## Summary
The Sane User Interface Library.

A simple UI building library written in C.  It has only a few dependencies, GLFW and cairo.

## API
### int sui_init()
### sui_keys *sui_keys_new(GLFWwindow *window)
### void sui_keys_register(sui_keys *keys, int key, void(*func)(int))
### void sui_keys_update(sui_keys *keys)
### node *sui_layer_new(sui_stage *stage)
### void sui_run(sui_stage *stage, sui_keys *keys)
### void sui_stage_add(sui_stage *stage, sui_widget *widget, node *layer)
### void sui_stage_draw(sui_stage *stage)
### sui_stage *sui_stage_new(GLFWwindow *window)
### void sui_stage_update(sui_stage *stage)
### void sui_terminate()
### sui_widget *sui_widget_new(void (*draw)(sui_widget*, cairo_t*))
