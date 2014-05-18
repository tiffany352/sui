#ifndef ELEM_H
#define ELEM_H

#include "internal.h"
#include "stage.h"

typedef struct sui_elem {
	sui_stage *stage;
	void (*draw)(struct sui_elem*, cairo_t*);
	void (*update)(struct sui_elem*, sui_stage*);
} sui_elem;

#endif
