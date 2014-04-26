# sui
The Sane User Interface Library.

## Introduction
sui is a library for building user interfaces by rendering cairo textures with OpenGL.

## Dependencies
sui has been designed to be windowing toolkit independent.  See the examples to determine how to use sui with different windowing toolkits.

* [GLEW](http://glew.sourceforge.net/)
* [cairo](http://cairographics.org/)

## API
### void sui_init(sui_stage *stage)
Initializes OpenGL state.

### sui_layer *sui_layer_new(sui_stage *stage)
Creates a new layer.  Widgets attached to this layer will appear on top of any widgets attached to previously created layers.

### void sui_run(sui_stage *stage, double x, double y, int down)
Updates and renders the user interface.

### void sui_stage_add(sui_stage *stage, sui_widget *widget, sui_layer *layer)
Adds a widgets to the stage.  The specified layer will determine the order of drawing.

### void sui_stage_draw(sui_stage *stage)
[INTERNAL] Draws the stage.

### sui_stage *sui_stage_new(int width, int height)
Creates a new stage with a specified width and height.

### void sui_stage_update(sui_stage *stage)
[INTERNAL] Updates a stage's state.

### void sui_terminate(sui_stage *stage)
Cleans up texture.
