#include <math.h>

#include "update.h"

/* the update function that is called every frame (unless paused) */
void update(float dt) {
	// remove contents and add update logic here

	// some example code
	for (int i = 0; i < app.obj_count; i++) {
		vec2 acc = {0, -10};
		objs[i].pos += objs[i].vel * dt;
		objs[i].vel += acc * dt;

		if (objs[i].pos.x < objs[i].w / 2) {
			objs[i].pos.x = objs[i].w / 2;
			objs[i].vel.x *= -1;
		} else if (objs[i].pos.x > app.sim_x - objs[i].w / 2) {
			objs[i].pos.x = app.sim_x - objs[i].w / 2;
			objs[i].vel.x *= -1;
		}
		if (objs[i].pos.y < objs[i].h / 2) {
			objs[i].pos.y = objs[i].h / 2;
			objs[i].vel.y *= -1;
		} else if (objs[i].pos.y > app.sim_y - objs[i].h / 2) {
			objs[i].pos.y = app.sim_y - objs[i].h / 2;
			objs[i].vel.y *= -1;
		}
	}
}