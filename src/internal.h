#ifndef INTERNAL_H
#define INTERNAL_H

#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include "uthash/utlist.h"
#include "uthash/utarray.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

typedef struct node {
	void *val;
	struct node *next, *prev;
} node;

#endif
