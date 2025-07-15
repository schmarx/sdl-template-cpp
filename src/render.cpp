#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "render.h"

#define CHAR_COUNT 150

enum ALIGNMENT {
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};

#define make_rect(x, y, w, h) {x, y, w, h}

#define set_white() SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)
#define set_black() SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)
#define set_red() SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255)
#define set_green() SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255)
#define set_blue() SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255)

typedef struct {
	SDL_Texture *tx;
	int w;
	int h;
} Texture;

Texture *LETTERS;
SDL_Texture *square_texture;
SDL_Texture *circle_texture;

char render_string[50];

// renders text onto the screen using loaded text assets
// align should be one of the ALIGNMENT enum
// offset is how many lines to lift or lower the text by
void render_text(char *text, int pos_x, int pos_y, int align, int offset_y, Texture font[]) {
	int offset_x = 0;

	if (align == ALIGN_RIGHT) {
		for (int i = 0; i < strlen(text); i++) {
			offset_x -= font[(int)text[i]].w;
		}
	} else if (align == ALIGN_CENTER) {
		for (int i = 0; i < strlen(text); i++) {
			offset_x -= font[(int)text[i]].w / 2;
		}
	}

	for (int i = 0; i < strlen(text); i++) {
		Texture l = font[(int)text[i]];
		SDL_Rect r = make_rect(pos_x + offset_x, pos_y - offset_y * font['a'].h, l.w, l.h);
		offset_x += l.w;

		SDL_RenderCopy(renderer, l.tx, NULL, &r);
	}
}

void render_init() {
	// ----- make font -----
	LETTERS = (Texture *)malloc(sizeof(Texture) * CHAR_COUNT);

	TTF_Init();
	TTF_Font *font_normal = TTF_OpenFont("./assets/slkscr.ttf", 32);

	SDL_Color color = {255, 255, 255, 255};
	for (int i = 1; i < CHAR_COUNT; i++) {
		char letter[10];
		sprintf(letter, "%c", i);

		SDL_Surface *surface = TTF_RenderText_Solid(font_normal, letter, color);

		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

		LETTERS[i].tx = texture;
		LETTERS[i].w = surface->w;
		LETTERS[i].h = surface->h;

		SDL_FreeSurface(surface);
	}

	TTF_CloseFont(font_normal);

	// ----- make circle texture -----
	int radius = 32;

	circle_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);
	SDL_SetTextureBlendMode(circle_texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, circle_texture);

	int resolution = 20;
	SDL_Vertex *vertices = (SDL_Vertex *)malloc(sizeof(SDL_Vertex) * (resolution + 1));
	int *indices = (int *)malloc(sizeof(int) * (3 * resolution));

	float rotation = 2 * PI;
	float interval = rotation / resolution;
	vertices[0].position.x = radius;
	vertices[0].position.y = radius;
	vertices[0].color = color;
	vertices[0].tex_coord.x = 0;
	vertices[0].tex_coord.y = 0;

	color.a = 255;
	for (int i = 1; i <= resolution; i++) {
		vertices[i].position.x = radius + radius * cosf(i * interval);
		vertices[i].position.y = radius + radius * sinf(i * interval);
		vertices[i].color = color;
		vertices[i].tex_coord.x = 0;
		vertices[i].tex_coord.y = 0;

		indices[(i - 1) * 3 + 0] = 0;
		indices[(i - 1) * 3 + 1] = i;
		indices[(i - 1) * 3 + 2] = i + 1;
		if (i == resolution) indices[(i - 1) * 3 + 2] = 1;
	}

	SDL_RenderGeometry(renderer, NULL, vertices, resolution + 1, indices, 3 * resolution);

	free(vertices);
	free(indices);

	// ----- end -----

	// ----- make square texture -----
	square_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);

	SDL_SetRenderTarget(renderer, square_texture);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, NULL);
	// ----- end -----

	SDL_SetRenderTarget(renderer, NULL);
}

void render_end() {
	for (int i = 0; i < CHAR_COUNT; i++) {
		SDL_DestroyTexture(LETTERS[i].tx);
	}
	free(LETTERS);
}

void render(float dt, int fps) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int offset = 0;
	sprintf(render_string, "%i objects", app.obj_count);
	render_text(render_string, window_x, 0, ALIGN_RIGHT, offset--, LETTERS);
	sprintf(render_string, "%i fps", fps);
	render_text(render_string, window_x, 0, ALIGN_RIGHT, offset--, LETTERS);

	for (int i = 0; i < app.obj_count; i++) {
		// render a particle at its center of mass
		SDL_FRect rect = {objs[i].pos.x - objs[i].w / 2, objs[i].pos.y - objs[i].h / 2, objs[i].w, objs[i].h};

		SDL_FPoint point = {objs[i].w / 2, objs[i].h / 2};
		SDL_RenderCopyExF(renderer, objs[i].texture, NULL, &rect, objs[i].rot, &point, SDL_FLIP_NONE);
	}

	SDL_RenderPresent(renderer);
}
