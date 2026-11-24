#include "app.h"
#include "fp_controller.h"
#include "map.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_W 1200
#define WINDOW_H 800
#define FPS 120

static void app_update(App *a);
static void app_render(App *a);

static void generate_map(App *a);

Model model;
Texture2D colormap;

void app_init(App *a) {
	InitWindow(WINDOW_W, WINDOW_H, "app");
	SetTargetFPS(FPS);

	fp_controller_init(&a->fpc);

	a->map = map_load_from_file("data/map.txt");

	DisableCursor();

	model = LoadModel("assets/kenney_city/Models/GLB format/building-f.glb");
	colormap = LoadTexture("assets/kenney_city/Models/GLB format/Textures/colormap.png");
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_ALBEDO, colormap);
}

void app_run(App *a) {
	while (!WindowShouldClose()) {
		app_update(a);
		app_render(a);
	}
}

void app_quit(App *a) {
	map_free(a->map);
	UnloadModel(model);
	UnloadTexture(colormap);

	CloseWindow();
}

// In-app functions

static void app_update(App *a) {
	fp_controller_update(&a->fpc, a->map);
}

static void app_render(App *a) {
	BeginDrawing();

	ClearBackground(WHITE);

	BeginMode3D(a->fpc.cam);

	DrawPlane((Vector3){0, -0.5f, 0}, (Vector2){20, 20}, LIGHTGRAY);
	DrawGrid(2000, 1);

	// generate_map(a);

	DrawModel(model, (Vector3){ 0, 0, 0 }, 10.0f, WHITE);

	EndMode3D();

	DrawFPS(10, 10);
	DrawText(
		TextFormat("x,y,z: (%.1f, %.1f, %.1f)", a->fpc.cam.position.x, a->fpc.cam.position.y, a->fpc.cam.position.z),
		10, 31, 20, BLACK
	);

	EndDrawing();
}

static void generate_map(App *a) {
	size_t map_ptr = 0;

	while (map_ptr < a->map->map_size) {
		float h = (float)a->map->data[map_ptr];

		int x = map_ptr % a->map->map_row_size;

		if (h != 0) {
			DrawCube((Vector3){ x, h/2, map_ptr / a->map->map_row_size }, 1, h, 1, RED);
			DrawCubeWires((Vector3){ x, h/2, map_ptr / a->map->map_row_size }, 1, h, 1, BLACK);
		}

		map_ptr++;
	}
}