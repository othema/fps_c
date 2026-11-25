#pragma once

#include "fp_controller.h"
#include "assets.h"

#include <raylib.h>

typedef struct {
	Assets assets;

	FPController fpc;
} App;

void app_init(App *a);
void app_run(App *a);
void app_quit(App *a);