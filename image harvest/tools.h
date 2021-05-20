#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define MAX3(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? \
            (n) : (p)))

#define MIN(a,b) ((a) < (b)  ? (a) : (b) )
#define MIN3(a,b,c) MIN(MIN(a,b),c)

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r, Uint8* g, \
    Uint8* b);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);


Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

Uint8 get_v(SDL_Surface *surface, unsigned x, unsigned y);

Uint8* get_rgb(SDL_Surface *surface, unsigned x, unsigned y);

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r, Uint8* g, \
        Uint8* b);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);


SDL_Surface* new_rgb_surface(int width, int height);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

void wait_for_keypressed();

SDL_Surface* load_image(char *path);

void init_sdl();

SDL_Surface* display_image(SDL_Surface *img);

int save_image(SDL_Surface* img, char *path);

#endif