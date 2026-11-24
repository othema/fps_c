#pragma once

#include <raylib.h>

typedef struct {
	Camera3D cam;
	Vector3 vel;

	float yaw, pitch;

	float max_pitch;
	float mouse_sensitivity;
} FPController;

void fp_controller_init(FPController *fpc);
void fp_controller_update(FPController *fpc);