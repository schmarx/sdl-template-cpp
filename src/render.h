#pragma once

#include <SDL2/SDL.h>

void render_init();
void render_end();

void render(float dt, int fps);

extern SDL_Texture *square_texture;
extern SDL_Texture *circle_texture;