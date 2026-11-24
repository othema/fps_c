#include "app.h"
#include "fp_controller.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define FPS 60

static void app_update(App *a);
static void app_render(App *a);

void app_init(App *a) {
	InitWindow(WINDOW_W, WINDOW_H, "app");
	SetTargetFPS(FPS);

	fp_controller_init(&a->fpc);

	DisableCursor();
}

void app_run(App *a) {
	while (!WindowShouldClose()) {
		app_update(a);
		app_render(a);
	}
}

void app_quit(App *a) {
	CloseWindow();
}

// In-app functions

static void app_update(App *a) {
	fp_controller_update(&a->fpc);
}

static void app_render(App *a) {
	BeginDrawing();

	ClearBackground(WHITE);

	DrawText("app", 10, 10, 20, BLACK);

	BeginMode3D(a->fpc.cam);

	DrawPlane((Vector3){0, -0.5f, 0}, (Vector2){10, 10}, LIGHTGRAY);
	DrawCube((Vector3){ 0, 0, 0 }, 1, 1, 1, RED);

	EndMode3D();

	DrawText(
		TextFormat("x,y,z: (%f, %f, %f)", a->fpc.cam.position.x, a->fpc.cam.position.y, a->fpc.cam.position.z),
		10, 10, 20, BLACK
	);
	EndDrawing();
}


// void update(Game *g) {
// 	UpdateCamera(&a->cam, CAMERA_FIRST_PERSON);
// }

// void render(Game *g) {
// 	BeginDrawing();

// 	ClearBackground(WHITE);

// 	// ----- 3D rendering -----
// 	BeginMode3D(a->cam);

// 	DrawPlane((Vector3){ 0, -1, 0 }, (Vector2){ 10, 10 }, LIGHTGRAY);

// 	DrawCube((Vector3){ 0, 0, 0 }, 2.0f, 2.0f, 2.0f, RED);
// 	DrawCubeWires((Vector3){ 0, 0, 0 }, 2.0f, 2.0f, 2.0f, BLACK);

// 	EndMode3D();
// 	// ----- End 3D rendering -----

// 	// Draw FPS
// 	const char *fps_text = TextFormat("FPS: %i", GetFPS());
// 	DrawText(fps_text, 20, 20, 0, BLACK);

// 	EndDrawing();

// }