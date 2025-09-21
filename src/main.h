#pragma once

#include <SDL2/SDL.h>

#include "lib/app.h"
#include "lib/vec.h"

void init(int argc, char *argv[]);
void end();
void handle_events();

/* the main class that contains information on objects */

class obj {
  private:
  public:
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

	obj(float r, float w, float h, float m);
	~obj();
};

extern obj *objs;
