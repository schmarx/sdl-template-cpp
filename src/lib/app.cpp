#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "../main.h"
#include "../render.h"
#include "../update.h"
#include "app.h"
#include "render.h"

#define BUF_SIZE 50

App app;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

timer timers[TIMER_COUNT];

App::App() : mouse_pos(0, 0) {
	time_ms = 0;
	step = 0;

	running = 1;
	pause = 0;
	debug = 1;

	click = 0;

	this->obj_count = obj_count;
}

App::~App() {
	// nothing to do
}

void t_start(int index) {
	timers[index].start = SDL_GetPerformanceCounter();
}

void t_end(int index) {
	timers[index].total += SDL_GetPerformanceCounter() - timers[index].start;
	timers[index].count++;

	if (timers[index].count == TIMER_UPDATE_FREQ) {
		timers[index].duration = 1000.0 * (timers[index].total / TIMER_UPDATE_FREQ) / (float)SDL_GetPerformanceFrequency();
		timers[index].total = 0;
		timers[index].count = 0;
	}
}

/* converts the window's mouse position to simulation coordinates */
vec2 get_mouse_pos() {
	float x = (app.mouse_pos.x - app.sim_margin.l) * app.scale;
	float y = ((app.win_y - app.mouse_pos.y) - app.sim_margin.t) * app.scale;

	if (x < 0) x = 0;
	else if (x > app.sim_x) x = app.sim_x;
	if (y < 0) y = 0;
	else if (y > app.sim_y) y = app.sim_y;

	vec2 mouse_pos = {x, y};
	return mouse_pos;
}

/* loads data from configuration file */
int load_config() {
	FILE *config_file = fopen("./config.txt", "r");
	if (config_file == NULL) {
		printf("no config file found");
		return 0;
	}

	int fullscreen = 1;

	char buf[BUF_SIZE];
	while (fgets(buf, BUF_SIZE, config_file)) {
		char *result = strchr(buf, '=');
		if (result != NULL) {
			int index = result - buf;
			float value = atof(result + 1);

			if (value == -1) continue;

			buf[index] = 0;

			if (strcmp(buf, "sim_scale") == 0) app.scale = value;
			else if (strcmp(buf, "fps") == 0) app.fps = value;
			else if (strcmp(buf, "margin_left") == 0) app.sim_margin.l = value;
			else if (strcmp(buf, "margin_right") == 0) app.sim_margin.r = value;
			else if (strcmp(buf, "margin_top") == 0) app.sim_margin.t = value;
			else if (strcmp(buf, "margin_bottom") == 0) app.sim_margin.b = value;
			else if (strcmp(buf, "seed") == 0) app.seed = value;
			else if (strcmp(buf, "N") == 0) app.obj_count = value;
			else if (strcmp(buf, "window_x") == 0) {
				app.win_x = value;
				fullscreen = 0;
			} else if (strcmp(buf, "window_y") == 0) {
				app.win_y = value;
				fullscreen = 0;
			}
		}
	}

	return fullscreen;
}

/* this initialises the app based on input */
void app_init(int argc, char *argv[]) {
	app.seed = time(NULL);

	// ----- set initial conditithis inions -----
	app.mouse_pos.x = 0;
	app.mouse_pos.y = 0;

	app.running = 1;
	app.debug = 1;
	app.click = 0;

	app.obj_count = 10;

	app.win_x = 800;
	app.win_y = 1000;

	app.scale = 1;

	// ----- apply arguments -----
	int fullscreen = load_config();
	srand(app.seed);
	printf("RNG seed: %i\n", app.seed);

	SDL_Init(SDL_INIT_EVERYTHING);

	if (argc > 1) app.obj_count = atoi(argv[1]); // if not specified, count will default to what it was originally initialised as
	if (argc > 3) {
		app.win_x = atoi(argv[2]);
		app.win_y = atoi(argv[3]);
		fullscreen = 0;
	} else if (fullscreen) {
		fullscreen = SDL_WINDOW_FULLSCREEN;

		SDL_Rect display;
		SDL_GetDisplayBounds(0, &display);

		app.win_x = display.w;
		app.win_y = display.h;
	}

	float x_change = app.sim_margin.l + app.sim_margin.r;
	float y_change = app.sim_margin.t + app.sim_margin.b;

	app.sim_x = app.scale * (app.win_x - x_change);
	app.sim_y = app.scale * (app.win_y - y_change);

	SDL_CreateWindowAndRenderer(app.win_x, app.win_y, SDL_WINDOW_SHOWN | fullscreen, &window, &renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	render_init();
	init(argc, argv);
}

/* this calls the program to an end */
void app_end() {
	end();
	render_end();
}

/* this runs the app, and includes a lot of default functionality such as performance capturing and pausing */
void app_run() {
	printf("\nrunning :)\n");

	float cap_fps = app.fps;  // frames per second to cap the program at
	float frame_cap_time = 0; // ms per frame to cap the program at
	if (cap_fps > 0) {
		frame_cap_time = 1000.0 / cap_fps;
	}

	float dt, used_dt;

	Uint64 curr_time;
	Uint64 prev_time = SDL_GetPerformanceCounter();
	while (app.running) {
		app.step = 0; // used for time-step reversal

		t_start(FRAME); // start frame timing

		// get time since last frame
		curr_time = SDL_GetPerformanceCounter();
		dt = (curr_time - prev_time) / (float)SDL_GetPerformanceFrequency();
		prev_time = curr_time;

		handle_events();

		// this determines the duration of the time steps
		// this can be replaced to use the time since the last frame
		used_dt = 0.01;

		if (!app.pause || app.step) {
			// only keep track of time when not in the pause state
			app.time_ms += (int)(1000.0 * dt);

			// handle reverse time-steps
			if (app.step == -1) used_dt *= -1;

			// run update, and time it
			t_start(UPDATE);
			update(used_dt);
			t_end(UPDATE);
		}
		app.step = 0;

		// run render, and time it
		t_start(RENDER);
		render(used_dt);
		t_end(RENDER);

		t_end(FRAME); // end frame timing

		float frame_time = (SDL_GetPerformanceCounter() - curr_time) / (float)SDL_GetPerformanceFrequency();
		// this caps the frame rate
		if (frame_time < frame_cap_time) {
			SDL_Delay(frame_cap_time - frame_time);
		}
	}
}

/* the entrypoint for the application */
void app_main(int argc, char *argv[]) {
	app_init(argc, argv);
	app_run();
	app_end();
}