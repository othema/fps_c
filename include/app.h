#pragma once

#include "fp_controller.h"
#include "map.h"

#include <raylib.h>

typedef struct {
	FPController fpc;

	Map *map;
} App;

void app_init(App *a);
void app_run(App *a);
void app_quit(App *a);