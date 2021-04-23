#ifndef CIRCLE_DETECTION
#define CIRCLE_DETECTION

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "structs.h"
#include "tools.h"
#include <math.h>
#include <stdio.h>

int center_block(int *resx1, int *resy1, int *resx2, int *resy2);
void circleDectection(SDL_Surface *img, int *resx, int *resy);
void circleDectection_staticadapt(SDL_Surface *img, int *resx, int *resy);
void circleDectection_dynamic(SDL_Surface *img, int *resx, int *resy);
void circleDectection2_staticadapt(SDL_Surface *img, int *cx, int *cy);

#endif