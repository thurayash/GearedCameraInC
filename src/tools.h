#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r, Uint8* g, Uint8* b);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);


Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

Uint8 get_v(SDL_Surface *surface, unsigned x, unsigned y);

Uint8* get_rgb(SDL_Surface *surface, unsigned x, unsigned y);

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r, Uint8* g, Uint8* b);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);


SDL_Surface* new_rgb_surface(int width, int height);

void update_surface(SDL_Surface* screen, SDL_Surface* image);


#endif
