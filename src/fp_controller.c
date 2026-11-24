#include "fp_controller.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define MOVEMENT_ACCEL 60.0f
#define MOVEMENT_DAMPING 6.0f

#define CAM_FOV 45.0f

static void fpc_move(FPController *fpc);
static void fpc_rotate(FPController *fpc);

static void fpc_move_xz(FPController *fpc, float *dt);
static void fpc_move_y(FPController *fpc, float *dt);

void fp_controller_init(FPController *fpc) {
	// Camera init
	fpc->cam.position = (Vector3){ 0.0f, 4.0f, 4.0f };
	fpc->cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	fpc->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	fpc->cam.fovy = CAM_FOV;
	fpc->cam.projection = CAMERA_PERSPECTIVE;

	fpc->vel = (Vector3) { 0 };

	fpc->yaw = 0;
	fpc->pitch = 0;

	fpc->mouse_sensitivity = 0.003f;
	fpc->max_pitch = 89.0f * DEG2RAD;

	fpc->hitbox = (Vector3){ 1, 2, 1 };
	fpc->gravity = 9.8f;

	UpdateCamera(&fpc->cam, CAMERA_CUSTOM);
}

void fp_controller_update(FPController *fpc) {
	fpc_move(fpc);
	fpc_rotate(fpc);
}

static void fpc_move(FPController *fpc) {
	float dt = GetFrameTime();

	fpc_move_xz(fpc, &dt);
	fpc_move_y(fpc, &dt);
}

static void fpc_move_xz(FPController *fpc, float *dt) {
	Vector2 inp = {0};

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) inp.y -= 1;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) inp.y += 1;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inp.x -= 1;
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inp.x += 1;

	// Prevent diagonal movement being faster
	if (Vector2LengthSqr(inp) > 0.0f) {
		inp = Vector2Normalize(inp);
	}

	Vector3 move_forward = { sinf(fpc->yaw), 0.0f, cosf(fpc->yaw) };
	Vector3 move_right = Vector3Normalize(Vector3CrossProduct(move_forward, fpc->cam.up));
	Vector3 wish_dir = Vector3Add(
		Vector3Scale(move_forward, -inp.y),
		Vector3Scale(move_right, inp.x)
	);

	fpc->vel.x += wish_dir.x * MOVEMENT_ACCEL * *dt;
	fpc->vel.z += wish_dir.z * MOVEMENT_ACCEL * *dt;

	float decay = expf(-MOVEMENT_DAMPING * *dt);
	fpc->vel.x *= decay;
	fpc->vel.z *= decay;

	// Update position
	fpc->cam.position.x += fpc->vel.x * *dt;
	fpc->cam.position.z += fpc->vel.z * *dt;
}

static void fpc_move_y(FPController *fpc, float *dt) {
	// Apply gravity
	if (fpc->cam.position.y - fpc->hitbox.y > -0.5f) {  // -0.5f being floor height
		fpc->vel.y -= fpc->gravity * *dt;
	} else {
		// Assume player grounded

		fpc->vel.y = 0;
		fpc->cam.position.y = -0.5f + fpc->hitbox.y;

		if (IsKeyDown(KEY_SPACE)) {
			fpc->vel.y = 6;
		}
	}

	// Update position
	fpc->cam.position.y += fpc->vel.y * *dt;
}

static void fpc_rotate(FPController *fpc) {
	Vector2 delta = GetMouseDelta();
	fpc->yaw -= delta.x * fpc->mouse_sensitivity;
	fpc->pitch -= delta.y * fpc->mouse_sensitivity;
	fpc->pitch = Clamp(fpc->pitch, -fpc->max_pitch, fpc->max_pitch);

	Vector3 forward = {
		cosf(fpc->pitch) * sinf(fpc->yaw),
		sinf(fpc->pitch),
		cosf(fpc->pitch) * cosf(fpc->yaw)
	};
	forward = Vector3Normalize(forward);

	fpc->cam.target = Vector3Add(fpc->cam.position, forward);
}