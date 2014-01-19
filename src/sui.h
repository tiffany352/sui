#ifndef SUI_H
#define SUI_H

#include "internal.h"
#include "stage.h"
#include "keys.h"

int sui_init();
void sui_run(sui_stage *stage, sui_keys *keys);
void sui_terminate();

#endif
