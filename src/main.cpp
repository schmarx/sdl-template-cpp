#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "render.h"
#include "sim.h"

obj *objs = NULL;
pthread_t *threads;
param *params;

App app;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

int window_x = 800;
int window_y = 1000;

timer timers[TIMER_COUNT];

void init_objects() {
	memset(objs, 0, sizeof(obj) * app.obj_count);

	// initialise objects here
	for (int i = 0; i < app.obj_count; i++) {
		objs[i].texture = circle_texture;
		objs[i].w = 20;
		objs[i].h = 20;

		objs[i].pos.x = rngr(0, window_x);
		objs[i].pos.y = rngr(0, window_y);
	}
}

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
				if (app.debug) app.debug = 0;
				else app.debug = 1;
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

void init(int argc, char *argv[]) {
	srand(time(NULL));

	// ----- set initial conditions -----
	app.mouse_pos.x = 0;
	app.mouse_pos.y = 0;

	app.running = 1;
	app.debug = 1;
	app.click = 0;

	app.obj_count = 10;

	SDL_Init(SDL_INIT_EVERYTHING);

	// ----- apply arguments -----
	int fullscreen = 0;
	if (argc > 1) app.obj_count = atoi(argv[1]); // if not specified, count will default to what it was originally initialised as
	if (argc > 3) {
		window_x = atoi(argv[2]);
		window_y = atoi(argv[3]);
	} else {
		fullscreen = SDL_WINDOW_FULLSCREEN;

		SDL_Rect display;
		SDL_GetDisplayBounds(0, &display);

		window_x = display.w;
		window_y = display.h;
	}

	SDL_CreateWindowAndRenderer(window_x, window_y, SDL_WINDOW_SHOWN | fullscreen, &window, &renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// ----- memory allocation -----
	objs = (obj *)calloc(app.obj_count, sizeof(obj));
	threads = (pthread_t *)calloc(thread_count, sizeof(pthread_t));
	params = (param *)calloc(thread_count, sizeof(params));

	render_init();
	init_objects();
}

void end() {
	free(objs);
	free(threads);
	free(params);

	render_end();
}

void run() {
	printf("\nrunning :)\n");

	int counter = 0;
	float frame_time_total = 0;
	float fps = 0;

	Uint64 prev_time = SDL_GetPerformanceCounter();
	while (app.running) {
		app.step = 0; // used for time-step reversal

		t_start(FRAME);

		Uint64 curr_time = SDL_GetPerformanceCounter();
		float dt = (curr_time - prev_time) / (float)SDL_GetPerformanceFrequency();
		prev_time = curr_time;

		handle_events();

		// this determines the duration of the time steps
		// this can be replaced to use the time since the last frame
		float used_dt = 0.01;

		if (!app.pause || app.step) {
			// only keep track of time when not in the pause state
			app.time_ms += (int)(1000.0 * dt);

			// handle reverse time-steps
			if (app.step == -1) used_dt *= -1;

			t_start(UPDATE);
			update(used_dt);
			t_end(UPDATE);
			t_log(UPDATE, counter);
		}
		app.step = 0;

		t_start(RENDER);
		render(used_dt, fps);
		t_end(RENDER);
		t_log(RENDER, counter);

		t_end(FRAME);
		t_log(FRAME, 0);

		float frame_time = timers[FRAME].duration;

		if (frame_time < 10) {
			SDL_Delay(10 - frame_time);
		}

		if (counter == TIMER_UPDATE_FREQ) {
			counter = 0;

			frame_time_total /= 100.0;
			fps = 1000.0 / frame_time_total;
			frame_time_total = 0;
		}
		frame_time_total += frame_time;
		counter++;
	}
}

int main(int argc, char *argv[]) {
	init(argc, argv);
	run();
	end();

	return EXIT_SUCCESS;
}