#include "app.h"

int main(void) {
	Game game;

	app_init(&game);
	app_run(&game);
	app_quit(&game);

	return 0;
}