#ifndef IMG_CONV_H
#define IMG_CONV_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "tools.h"

void to_ycbcr(int r, int g, int b, float *y, float *cb, float *cr);

void image_conversion(SDL_Surface* image);
#endif
