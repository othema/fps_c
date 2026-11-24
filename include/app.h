#pragma once

#include "fp_controller.h"

#include <raylib.h>

typedef struct {
	FPController fpc;
} App;

void app_init(App *a);
void app_run(App *a);
void app_quit(App *a);