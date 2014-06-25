# API
The following is the sui API organized by general category.

* [Core](#core)
* [Layer](#layer)
* [Stage](#stage)
* [Button](#button)
* [Image](#image)
* [Rect](#rect)
* [Text](#text)
* [Internal](#internal)

## Core
### sui_init

	void sui_init(sui_stage *stage);

Initializes OpenGL state.  This should be called once before the program begins.

### sui_terminate

	void sui_terminate(sui_stage *stage);

Cleans up the texture.  This should be called once after all logic has finished.

### sui_run

	void sui_run(sui_stage *stage, double x, double y, int down);

Updates and renders the user interface.

## Layer
### sui_layer_new

	sui_layer *sui_layer_new(sui_stage *stage);

Creates a new layer.  Elements attached to this layer will appear on top of any elements attached to previously created layers.

## Stage
### sui_stage_add

	void sui_stage_add(sui_stage *stage, struct sui_elem *elem, sui_layer *layer);

Adds a element to the stage.  The specified layer will determine the order of drawing.

### sui_stage_new

	sui_stage *sui_stage_new(int width, int height);

Creates a new stage with a specified width and height.

## Button
### sui_button_new

	sui_button *sui_button_new(int x, int y, int width, int height);

Creates a button.

### sui_button_set_position

	void sui_button_set_position(sui_button *button, int x, int y);

Sets a button's position.

### sui_button_up

	void sui_button_up(sui_button *button, void(*callback)(sui_button *self));

Set callback to be called when the cursor has released on the button.

### sui_button_down

	void sui_button_down(sui_button *button, void(*callback)(sui_button *self));

Set callback to be called when the cursor has pressed down on the button.

### sui_button_enter

	void sui_button_enter(sui_button *button, void(*callback)(sui_button *self));

Set callback to be called when the cursor has entered the button.

### sui_button_leave

	void sui_button_leave(sui_button *button, void(*callback)(sui_button *self));

Set callback to be called when the cursor has left the button.

## Image
### sui_image_new

	sui_image *sui_image_new(int x, int y, int width, int height, char *source);

Creates an image.

### sui_image_set_position

	void sui_image_set_position(sui_image *image, int x, int y);

Sets an image's position.

### sui_image_set_size

	void sui_image_set_size(sui_image *image, int width, int height);

Sets an image's size.

## Rect
### sui_rect_new

	sui_rect *sui_rect_new(int x, int y, int width, int height);

Creates a rectangle.

### sui_rect_set_position

	void sui_rect_set_position(sui_rect *rect, int x, int y);

Sets a rectangle's position.

### sui_rect_set_size

	void sui_rect_set_size(sui_rect *rect, int width, int height);

Sets a rectangle's size.

### sui_rect_set_color

	void sui_rect_set_color(sui_rect *rect, float r, float g, float b, float a);

Sets a rectangle's color.

## Text
### sui_text_new

	sui_text *sui_text_new(int x, int y, char *content);

Creates text.

### sui_text_set_position

	void sui_text_set_position(sui_text *text, int x, int y);

Sets a text's position.

### sui_text_set_content

	void sui_text_set_content(sui_text *text, char *content);

Sets a text's content.

### sui_text_set_color

	void sui_text_set_color(sui_text *text, float r, float g, float b, float a);

Sets a text's color.

## Internal
### _sui_stage_draw

	void _sui_stage_draw(sui_stage *stage);

Draws the stage.

### _sui_stage_update

	void _sui_stage_update(sui_stage *stage);

Updates a stage's state.
