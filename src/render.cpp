#include "render.h"

/* use this to correctly position objects within the simulation */
SDL_FRect sim_rect(float x, float y, float w, float h) {
	float pos_x = (x - w / 2) / app.scale;
	float pos_y = (app.sim_y - (y + h / 2)) / app.scale; // by default 0 is at the top, so this is flipped first

	SDL_FRect rect = {app.sim_margin.l + pos_x, app.sim_margin.t + pos_y, w / app.scale, h / app.scale};
	return rect;
}

void draw_ui() {
	int y_layer = 0; // the text layer, starting at the top of the screen
	render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "%i objects", app.obj_count);

	if (app.fps > 0) {
		render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "%i fps (capped %i)", (int)(1000.0 / timers[FRAME].duration), (int)app.fps);
	} else {
		render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "%i fps", (int)(1000.0 / timers[FRAME].duration));
	}

	render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "update %.2fms", timers[UPDATE].duration);
	render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "render %.2fms", timers[RENDER].duration);
	render_text(app.win_x, 0, ALIGN_RIGHT, y_layer--, LETTERS, "total  %.2fms", timers[FRAME].duration);

	y_layer = 0;
	render_text(0, 0, ALIGN_LEFT, y_layer--, LETTERS, "window size (%i, %i)", app.win_x, app.win_y);
	render_text(0, 0, ALIGN_LEFT, y_layer--, LETTERS, "sim size (%.2f, %.2f)", app.sim_x, app.sim_y);

	vec2 mouse_pos = get_mouse_pos();
	render_text(0, 0, ALIGN_LEFT, y_layer--, LETTERS, "mouse (%.1f, %.1f)", mouse_pos.x, mouse_pos.y);
}

void draw_objects() {
	for (int i = 0; i < app.obj_count; i++) {
		// render a particle at its center of mass
		SDL_FRect rect = sim_rect(objs[i].pos.x, objs[i].pos.y, objs[i].w, objs[i].h);

		SDL_FPoint point = {objs[i].w / 2, objs[i].h / 2};
		SDL_RenderCopyExF(renderer, objs[i].texture, NULL, &rect, objs[i].rot, &point, SDL_FLIP_NONE);
	}
}

/* the render function that is called every frame */
void render(float dt) {
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_FRect sim_box = {app.sim_margin.l, app.sim_margin.t, app.sim_x / app.scale, app.sim_y / app.scale};
	SDL_RenderFillRectF(renderer, &sim_box);

	draw_objects();
	draw_ui();

	SDL_RenderPresent(renderer);
}
