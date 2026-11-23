#include "app.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define FPS 60

void app_init(Game *g) {
	InitWindow(WINDOW_W, WINDOW_H, "app");
	SetTargetFPS(FPS);
}

void app_run(Game *g) {
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		EndDrawing();
	}
}

void app_quit(Game *g) {
	CloseWindow();
}

// int main(void) {
	

// 	Game g;

// 	// Camera setup
// 	g.cam = (Camera3D){ 0 };
// 	g.cam.position = (Vector3){ 0.0f, 2.0f, 10.0f };
// 	g.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
// 	g.cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
// 	g.cam.fovy = 45.0f;
// 	g.cam.projection = CAMERA_PERSPECTIVE;

// 	DisableCursor();

// 	while (!WindowShouldClose()) {
// 		update(&g);
// 		render(&g);
// 	}

// 	CloseWindow();

// 	return 0;
// }

// void update(Game *g) {
// 	UpdateCamera(&g->cam, CAMERA_FIRST_PERSON);
// }

// void render(Game *g) {
// 	BeginDrawing();

// 	ClearBackground(WHITE);

// 	// ----- 3D rendering -----
// 	BeginMode3D(g->cam);

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