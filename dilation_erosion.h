#ifndef DILATION_EROSION_H_
#define DILATION_EROSION_H_

#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <pixel.h>

void dilate(SDL_Surface* image);
void erode(SDL_Surface *image);

#endif
