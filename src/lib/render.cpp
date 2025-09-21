#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "render.h"

#define CHAR_COUNT 150

Texture *LETTERS;
SDL_Texture *square_texture;
SDL_Texture *circle_texture;

char render_string[150];

/*
renders text onto the screen using loaded text assets
align should be one of the ALIGNMENT enum
offset is how many lines to lift or lower the text by
*/
void render_text(int pos_x, int pos_y, int align, int offset_y, Texture font[], const char *text, ...) {

	va_list args;
	va_start(args, text);
	vsprintf(render_string, text, args);
	va_end(args);

	int offset_x = 0;
	if (align == ALIGN_RIGHT) {
		for (size_t i = 0; i < strlen(render_string); i++) {
			offset_x -= font[(int)render_string[i]].w;
		}
	} else if (align == ALIGN_CENTER) {
		for (size_t i = 0; i < strlen(render_string); i++) {
			offset_x -= font[(int)render_string[i]].w / 2;
		}
	}

	for (size_t i = 0; i < strlen(render_string); i++) {
		Texture l = font[(int)render_string[i]];
		SDL_Rect r = make_rect(pos_x + offset_x, pos_y - offset_y * font['a'].h, l.w, l.h);
		offset_x += l.w;

		SDL_RenderCopy(renderer, l.tx, NULL, &r);
	}
}

/* initialises various textures */
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

/* frees textures */
void render_end() {
	for (int i = 0; i < CHAR_COUNT; i++) {
		SDL_DestroyTexture(LETTERS[i].tx);
	}
	free(LETTERS);

	SDL_DestroyTexture(circle_texture);
	SDL_DestroyTexture(square_texture);
}

/* conversion of (hue, saturation, value) to (red, green, blue) color spaces */
RGB hsv_to_rgb(float hue, float sat, float val) {
	float C = sat * val;
	float H = hue / 60;

	float H_mod = H - 2 * ((int)(H / 2));
	float X = C * (1 - fabsf(H_mod - 1));

	float m = val - C;

	RGB res = {m, m, m};

	if (H < 1) {
		res.r += C;
		res.g += X;
		res.b += 0;
	} else if (H < 2) {
		res.r += X;
		res.g += C;
		res.b += 0;
	} else if (H < 3) {
		res.r += 0;
		res.g += C;
		res.b += X;
	} else if (H < 4) {
		res.r += 0;
		res.g += X;
		res.b += C;
	} else if (H < 5) {
		res.r += X;
		res.g += 0;
		res.b += C;
	} else {
		res.r += C;
		res.g += 0;
		res.b += X;
	}

	res.r *= 255;
	res.g *= 255;
	res.b *= 255;

	return res;
}