#include <SDL/SDL.h>
#include "ycbcr.h"

void *to_ycbcr(Uint8 r, Uint8 g, Uint8 b, int *y, int *cb, int *cr)
{
    *y = (int)(0.299 * r + 0.587 * g + 0.114 * b);
    *cb = (int)(-0.16874 * r - 0.33126 * g + 0.50000 * b);
    *cr = (int)(0.50000 * r - 0.41869 * g - 0.08131 * b);
}