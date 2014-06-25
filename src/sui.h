#ifndef SUI_H
#define SUI_H

#include "internal.h"
#include "stage.h"
#include "keys.h"
#include "elem.h"

void sui_init(sui_stage *stage);
void sui_run(sui_stage *stage, double x, double y, int down);
void sui_terminate(sui_stage *stage);

#endif
