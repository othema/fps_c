#include "app.h"
#include "fp_controller.h"

#include "map/mapgen.h"
#include "map/render.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define WINDOW_W 1200
#define WINDOW_H 800
#define FPS 120

#define MODEL_SCALE 10.0f

static void app_update(App *a);
static void app_render(App *a);

Map *map;

void app_init(App *a) {
	SetTraceLogLevel(LOG_ERROR);

	InitWindow(WINDOW_W, WINDOW_H, "app");
	SetTargetFPS(FPS);

	fp_controller_init(&a->fpc);

	DisableCursor();

	map = mapgen(3, 1);
}

void app_run(App *a) {
	while (!WindowShouldClose()) {
		app_update(a);
		app_render(a);
	}
}

void app_quit(App *a) {
	map_free(map);

	CloseWindow();
}

// In-app functions

static void app_update(App *a) {
	fp_controller_update(&a->fpc);
}

static void app_render(App *a) {
	BeginDrawing();

	ClearBackground(WHITE);

	BeginMode3D(a->fpc.cam);

	DrawGrid(2000, 1);

	map_render((Vector3){ 0, 0, 0}, map);

	EndMode3D();

	DrawFPS(10, 10);
	DrawText(
		TextFormat("x,y,z: (%.1f, %.1f, %.1f)", a->fpc.cam.position.x, a->fpc.cam.position.y, a->fpc.cam.position.z),
		10, 31, 20, BLACK
	);

	EndDrawing();
}
