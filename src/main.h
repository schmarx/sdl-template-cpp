#pragma once

#include "vec.h"
#include <SDL2/SDL.h>
#include <pthread.h>

#define thread_count 1
#define PI 3.1415926535897932384626433832795

// ----- timing -----
// timer values will only be updated every TIMER_UPDATE_FREQ'th frame
// this is so that things like FPS counts don't just become a flicker of constantly changing values
#define TIMER_UPDATE_FREQ 50

#define t_start(index) timers[index].start = SDL_GetPerformanceCounter()
#define t_end(index) timers[index].end = SDL_GetPerformanceCounter()
#define t_log(index, counter) timers[index].duration = (((counter) % TIMER_UPDATE_FREQ == 0) ? (1000.0 * (timers[index].end - timers[index].start) / (float)SDL_GetPerformanceFrequency()) : timers[index].duration)

// ----- RNG -----
#define rng(max) rand() % (max + 1)
#define rngr(min, max) (rng((max - min)) + min)
#define rngf() ((float)rand() / (float)RAND_MAX)

enum TIMERS {
	UPDATE,
	RENDER,
	FRAME,

	// ----- count -----
	TIMER_COUNT // make sure this is always the last in the enum, and that the enum starts at 0
};

extern int window_x;
extern int window_y;

extern SDL_Renderer *renderer;

typedef struct {
	SDL_Texture *texture;

	vec2 pos;
	vec2 vel;
	vec2 acc;

	float ang_vel; // angular velocity
	float rot;

	float r; // radius
	float w; // width
	float h; // height

	float m; // mass
} obj;

typedef struct {
	// global time
	int time_ms;
	int step;

	// running states
	int running;
	int pause;
	int debug;

	// mouse related
	int click;
	vec2 mouse_pos;

	// object count
	int obj_count;
} App;

// params for multithreading
typedef struct {
	int start;
	int end;
	float dt;
} param;

// used for timing
typedef struct {
	Uint64 start;
	Uint64 end;

	float duration;
} timer;

extern obj *objs;
extern App app;
extern timer timers[TIMER_COUNT];

extern pthread_t *threads;
extern param *params;
