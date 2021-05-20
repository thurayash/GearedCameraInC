#ifndef ROBERTS_EDGE
#define ROBERTS_EDGE

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>

void to_rob(SDL_Surface *image, int);

SDL_Surface *first_rob(SDL_Surface *image);


Uint8 Threshold_value(SDL_Surface* image, int width, int height,
        unsigned long *histo);

#endif
