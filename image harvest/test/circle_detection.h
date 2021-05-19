#ifndef CIRCLE_DETECTION
#define CIRCLE_DETECTION

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "structs.h"
#include "tools.h"
#include <math.h>

void circleDectection(SDL_Surface *img, int *resx, int *resy);
void circleDectection_staticadapt(SDL_Surface *img, int *resx, int *resy);
void circleDectection_dynamic(SDL_Surface *img, int *resx, int *resy);


#endif