#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdlib.h>
#include <SDL/SDL.h>

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

Uint8 get_v(SDL_Surface *surface, unsigned x, unsigned y);

Uint8* get_rgb(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

SDL_Surface* new_rgb_surface(int width, int height);

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r, Uint8* g, Uint8* b); // Get the r,g and b of x and y form an sdl surface


#endif
