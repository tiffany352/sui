#include "widget.h"

sui_widget *sui_widget_new(void (*draw)(sui_widget*, cairo_t*))
{
    sui_widget *widget;

    widget = malloc(sizeof(sui_widget));
    widget->draw = draw;
    widget->update = NULL;

    return widget;
}
