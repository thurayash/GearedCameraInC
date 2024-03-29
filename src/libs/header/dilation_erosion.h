#ifndef DILATION_EROSION_H_
#define DILATION_EROSION_H_

#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include "tools.h"


void sett_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface* dilate_cross(SDL_Surface* image);
SDL_Surface* erode_cross(SDL_Surface* image);

void dilate_square(SDL_Surface* image, SDL_Surface* result);
void erode_square(SDL_Surface* image, SDL_Surface* result);


void dilate_square_red(SDL_Surface* image, SDL_Surface* result);
#endif
