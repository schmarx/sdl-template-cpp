#pragma once

#include <SDL2/SDL.h>

#include "../main.h"

#define make_rect(x, y, w, h) {x, y, w, h}

// some functions to set render colors
#define set_white() SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)
#define set_black() SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)
#define set_red() SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255)
#define set_green() SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255)
#define set_blue() SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255)

enum ALIGNMENT {
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};

/* holds basic texture information */
typedef struct {
	SDL_Texture *tx;
	int w;
	int h;
} Texture;

typedef struct {
	float r;
	float g;
	float b;
} RGB;

void render_init();
void render_end();
void render_text(int pos_x, int pos_y, int align, int offset_y, Texture font[], char *text, ...);

RGB hsv_to_rgb(float hue, float sat, float val);

extern SDL_Texture *square_texture;
extern SDL_Texture *circle_texture;

extern Texture *LETTERS;