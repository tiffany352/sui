#ifndef INTERNAL_H
#define INTERNAL_H

#include <GLFW/glfw3.h>
#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include "uthash/utlist.h"
#include "uthash/utarray.h"

typedef struct node {
	void *val;
	struct node *next, *prev;
} node;

#endif
