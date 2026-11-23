#include <raylib.h>

typedef struct {
	Camera3D cam;
} Game;

void app_init(Game *g);
void app_run(Game *g);
void app_quit(Game *g);