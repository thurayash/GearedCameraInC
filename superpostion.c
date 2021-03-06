#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <pixel.h> //this may be tools in the master branch



void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  Uint32 *target_pixel = (Uint8 *) surface->pixels + y * surface->pitch +
                                                     x * sizeof *target_pixel;
  *target_pixel = pixel;
}


//majority sort : if two pictures indicate pixel[i][j] == 1, it is a face pixel
void superpostion1(SDL_Surface* image1, SDL_Surface*  image2, SDL_Surface* image3)
{
    SDL_LockSurface(image1);
    SDL_LockSurface(image2);
    SDL_LockSurface(image3);
    Uint32 black = 0xff;
    Uint32 white = 0x00;
    for (int i = 0; i<image1->w; i++)
    {
        //no need to check if all are 255 or 0 since this is after BW
        for (int j = 0; j<image1->h; j++)
        {
            Uint32 pixel1 = get_pixel(image1, i, j);
            Uint32 pixel2 = get_pixel(image2, i, j);
            Uint32 pixel3 = get_pixel(image3, i, j);
            if (pixel1== white) // or 255 if whiteblack pixel info instead of binary
            {
                if (pixel2 == white || pixel3 == white)
                {
                    set_pixel(image1, i,j,white);
                }
            }
            else if (pixel2 == white && pixel3 == white) //or 255 if we go whiteblack)
            {
                set_pixel(image1, i,j,white);
            }
        }
    }
}

//selective sort : if all 3 pictures indicate pixel[i][j] == 1, it is a face pixel
void superpostion2(SDL_Surface* image1, SDL_Surface*  image2, SDL_Surface* image3)
{
    SDL_LockSurface(image1);
    SDL_LockSurface(image2);
    SDL_LockSurface(image3);
    Uint32 black = 0xff;
    Uint32 white = 0x00;
    for (int i = 0; i < image1->w; i++)
    {
        for (int j = 0; j < image1->h; j++)
        {
            Uint32 pixel1 = get_pixel(image1, i, j);
            Uint32 pixel2 = get_pixel(image2, i, j);
            Uint32 pixel3 = get_pixel(image3, i, j);
            if (pixel1 == white
                && image2[i][j] == white // It would'nt work the structure don't let you do this kind of things
                && image3[i][j] ==white) // You need to first get_pixel, get RGB and the compare the R, G and B
            {
                set_pixel(image1,i,j,white);
            }
        }
    }
}
