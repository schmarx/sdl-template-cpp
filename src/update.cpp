#include <math.h>
#include <pthread.h>

#include "update.h"

void update(float dt) {
	// remove contents and add update logic here

	// some example code
	for (int i = 0; i < app.obj_count; i++) {
		vec2 acc = {0, 500};
		objs[i].pos = objs[i].pos + (objs[i].vel * dt);
		objs[i].vel = objs[i].vel + (acc * dt);

		if (objs[i].pos.y + objs[i].h > window_y) {
			objs[i].pos.y = window_y - objs[i].h;
			objs[i].vel.y *= -1;
		}
	}
}