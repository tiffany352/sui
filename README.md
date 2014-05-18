# sui
The Sane User Interface Library.

## Introduction
sui is a library for building user interfaces by rendering cairo textures with OpenGL.

## Dependencies
sui has been designed to be windowing toolkit independent.  See the examples to determine how to use sui with different windowing toolkits.

* [GLEW](http://glew.sourceforge.net/)
* [cairo](http://cairographics.org/)

## API
### void _sui_stage_draw(sui_stage *stage);
[INTERNAL] Draws the stage.

### void _sui_stage_update(sui_stage *stage);
[INTERNAL] Updates a stage's state.

### void sui_button_down(sui_button *button, void(*callback)(sui_button *self));
Set callback to be called when the cursor has pressed down on the button.

### void sui_button_enter(sui_button *button, void(*callback)(sui_button *self));
Set callback to be called when the cursor has entered the button.

### void sui_button_leave(sui_button *button, void(*callback)(sui_button *self));
Set callback to be called when the cursor has left the button.

### sui_button *sui_button_new(int x, int y, int width, int height);
Creates a button.

### void sui_button_set_position(sui_button *button, int x, int y);
Sets a button's position.

### void sui_button_up(sui_button *button, void(*callback)(sui_button *self));
Set callback to be called when the cursor has released on the button.

### sui_image *sui_image_new(int x, int y, int width, int height, char *source);
Creates an image.

### void sui_image_set_position(sui_image *image, int x, int y);
Sets an image's position.

### void sui_image_set_size(sui_image *image, int width, int height);
Sets an image's size.

### void sui_init(sui_stage *stage);
Initializes OpenGL state.

### sui_layer *sui_layer_new(sui_stage *stage);
Creates a new layer.  Elements attached to this layer will appear on top of any elements attached to previously created layers.

### sui_rect *sui_rect_new(int x, int y, int width, int height);
Creates a rectangle.

### void sui_rect_set_color(sui_rect *rect, float r, float g, float b, float a);
Sets a rectangle's color.

### void sui_rect_set_position(sui_rect *rect, int x, int y);
Sets a rectangle's position.

### void sui_rect_set_size(sui_rect *rect, int width, int height);
Sets a rectangle's size.

### void sui_run(sui_stage *stage, double x, double y, int down);
Updates and renders the user interface.

### void sui_stage_add(sui_stage *stage, struct sui_elem *elem, sui_layer *layer);
Adds a element to the stage.  The specified layer will determine the order of drawing.

### sui_stage *sui_stage_new(int width, int height);
Creates a new stage with a specified width and height.

### void sui_terminate(sui_stage *stage);
Cleans up texture.
