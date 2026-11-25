#pragma once

#include <raylib.h>

#define MOVEMENT_ACCEL 60.0f
#define MOVEMENT_DAMPING 12.0f

#define CAM_FOV 80.0f

typedef struct {
	Camera3D cam;
	Vector3 vel;

	float yaw, pitch;

	float max_pitch;
	float mouse_sensitivity;

	float gravity;

	Vector3 hitbox;
} FPController;

void fp_controller_init(FPController *fpc);
void fp_controller_update(FPController *fpc);
