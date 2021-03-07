#ifndef SUPERPOSTION_H_
#define SUPERPOSTION_H_

#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include "pixel.h"
#include <stdlib.h>
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void superpostion1(SDL_Surface*, SDL_Surface* image2, SDL_Surface* image3);
void superpostion2(SDL_Surface*, SDL_Surface* image2, SDL_Surface* image3);

#endif
