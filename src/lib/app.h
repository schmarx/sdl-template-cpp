#pragma once

#include <SDL2/SDL.h>

#include "vec.h"

#define PI 3.1415926535897932384626433832795

// ----- timing -----
// timer values will only be updated every TIMER_UPDATE_FREQ'th frame
// this is so that things like FPS counts don't just become a flicker of constantly changing values
#define TIMER_UPDATE_FREQ 50

// ----- RNG -----
#define rng(max) rand() % ((max) + 1)
#define rngr(min, max) (rng(((max) - (min))) + (min))
#define rngf() ((float)rand() / (float)RAND_MAX)
#define rngfr(min, max) ((min) + ((max) - (min)) * rngf())

enum TIMERS {
	UPDATE,
	RENDER,
	FRAME,

	// ----- count -----
	TIMER_COUNT // make sure this is always the last in the enum, and that the enum starts at 0
};

/* box properties */
typedef struct {
	float t; // top
	float b; // bottom
	float l; // left
	float r; // right
} BOX;

/* a useful place to store program information */
class App {
  private:
  public:
	// global time
	int time_ms;
	int step;
	float fps;

	// running states
	int running;
	int pause;
	int debug;

	int seed; // the RNG seed

	// mouse related
	int click;
	vec2 mouse_pos;

	// object count
	int obj_count;

	// ----- window sizing -----
	int win_x; // width of the window
	int win_y; // height of the window

	// ----- simulation sizing -----
	float sim_x;	// width of the simulation
	float sim_y;	// height of the simulation
	BOX sim_margin; // the margins between the simulation and the window borders
	float scale;	// the units that the simulation will be scaled by

	App();
	~App();
};

// used for timing
typedef struct {
	Uint64 start;

	float duration;
	float total;
	int count;
} timer;

void t_start(int index);
void t_end(int index);

vec2 get_mouse_pos();

void app_main(int argc, char *argv[]);

extern SDL_Renderer *renderer;

extern App app;
extern timer timers[TIMER_COUNT];