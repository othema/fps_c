#include "app.h"

int main(void) {
	App game;

	app_init(&game);
	app_run(&game);
	app_quit(&game);

	return 0;
}