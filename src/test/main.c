#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../tools.h"
#include "rgb_to_hsi.h"
#include <err.h>


void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}




SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


int save_image(SDL_Surface* img, char *path)
{
       return SDL_SaveBMP(img, path);
}


SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}





void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}





void SDL_FreeSurface(SDL_Surface *surface);


void greyscale(SDL_Surface *img)
{
    Uint32 pixel;
    Uint8 r, g, b, Lum;
    for (int i = 0; i < img->w; ++i)
    {
        for (int j = 0; j < img->h; ++j)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            Lum = (r + g + b) / 3;
            put_pixel(img, i, j, SDL_MapRGB(img->format, Lum, Lum, Lum));
        }
    }
}



void image_conversion(SDL_Surface* image)
{
    size_t width = image->w;
    size_t height = image->h;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            Uint8 r,g,b;
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            float H,S,I;
            rgb_to_hsi(r, g, b, &H , &S ,&I);

            /*printf("H : %f, S : %f, I : %f | R : %u, G : %u, B : %u\n"\
                    ,H,S,I, r,g,b);*/
            if (( 0 <= H && H <= 179) && (0 <= S && S <= 197) && \
                    (0 <= I && 92 <= I))
                continue;
            else
                pixel = SDL_MapRGB(image->format, 0, 0, 0);
            put_pixel(image, i, j, pixel);
        }
    }
}




int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Add picture");
        return -1;
    }

    init_sdl();
    SDL_Surface* image = load_image(argv[1]);
    SDL_Surface* screen;

    screen = display_image(image);

    wait_for_keypressed();

    image_conversion(image);

    screen = display_image(image);

    wait_for_keypressed();

    save_image(image, "test.bmp");
    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
}
