#ifndef HSI_TO_BW_H_
#define HSI_TO_BW_H_

#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>


void hsi_to_bw(Uint8 *r, Uint8 *g, Uint8 *b, int *cb, int *cr);
void hsi_to_bw_vrgb(Uint8 *r, Uint8 *g, Uint8 *b);


#endif