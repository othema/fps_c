#include "fp_controller.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define MOVEMENT_ACCEL 60.0f
#define MOVEMENT_DAMPING 6.0f

#define CAM_FOV 45.0f

static void update_cam_movement(FPController *fpc);
static void update_cam_rotation(FPController *fpc);

void fp_controller_init(FPController *fpc) {
	// Camera init
	fpc->cam.position = (Vector3){ 0.0f, 1.0f, 4.0f };
	fpc->cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	fpc->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	fpc->cam.fovy = CAM_FOV;
	fpc->cam.projection = CAMERA_PERSPECTIVE;

	fpc->yaw = 0;
	fpc->pitch = 0;

	fpc->mouse_sensitivity = 0.003f;
	fpc->max_pitch = 89.0f * DEG2RAD;

	UpdateCamera(&fpc->cam, CAMERA_CUSTOM);
}

void fp_controller_update(FPController *fpc) {
	update_cam_movement(fpc);
	update_cam_rotation(fpc);
}

static void update_cam_movement(FPController *fpc) {
	float dt = GetFrameTime();

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

	fpc->vel.x += wish_dir.x * MOVEMENT_ACCEL * dt;
	fpc->vel.z += wish_dir.z * MOVEMENT_ACCEL * dt;

	float decay = expf(-MOVEMENT_DAMPING * dt);
	fpc->vel.x *= decay;
	fpc->vel.z *= decay;

	fpc->cam.position.x += fpc->vel.x * dt;
	fpc->cam.position.z += fpc->vel.z * dt;
}

static void update_cam_rotation(FPController *fpc) {
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