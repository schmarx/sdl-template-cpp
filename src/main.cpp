#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "lib/app.h"
#include "lib/render.h"
#include "main.h"

SDL_Event event;

obj *objs = NULL;

/*
this is where objects (as defined in main.h) are initialised
this will run again when the app is refreshed with [r]
*/
void init_objects() {
	memset(objs, 0, sizeof(obj) * app.obj_count);

	// initialise objects here
	float r = 20;
	for (int i = 0; i < app.obj_count; i++) {
		objs[i].texture = circle_texture;
		objs[i].w = 2 * r;
		objs[i].h = 2 * r;

		objs[i].pos.x = rngfr(r, app.sim_x - r);
		objs[i].pos.y = rngfr(r, app.sim_y - r);

		objs[i].vel.x = rngfr(-100, 100);
	}
}

/* input handling */
void handle_events() {
	// ----- input handling -----
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			app.running = 0;
			break;
		} else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			default:
				break;
			}
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_q:
				app.running = 0;
				break;
			case SDLK_ESCAPE:
				app.pause = !app.pause;
				break;
			case SDLK_r:
				init_objects();
				break;
			case SDLK_g:
				app.debug = !app.debug;
				break;
			case SDLK_RIGHT:
				app.step = 1;
				break;
			case SDLK_LEFT:
				app.step = -1;
				break;
			default:
				break;
			}
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			app.click = 1;
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			app.click = 0;
		} else if (event.type == SDL_MOUSEMOTION) {
			app.mouse_pos.x = event.button.x;
			app.mouse_pos.y = event.button.y;
		}
	}
}

/* initialisation that will run only once at the start */
void init(int argc, char *argv[]) {
	objs = (obj *)calloc(app.obj_count, sizeof(obj));

	init_objects();
}

/* shutdown that will run only once at the end */
void end() {
	free(objs);
}

/* program entrypoint */
int main(int argc, char *argv[]) {
	app_main(argc, argv);

	return EXIT_SUCCESS;
}