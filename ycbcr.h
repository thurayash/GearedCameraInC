#ifndef YCBCR_H
#define YCBCR_H

#include <SDL/SDL.h>

void to_ycbcr(Uint8 r, Uint8 g, Uint8 b, int *y, int *cb, int *cr);

#endif