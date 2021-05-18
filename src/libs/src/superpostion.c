#include "libs/header/superpostion.h"

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  Uint32 *target_pixel = surface->pixels + y * surface->pitch +
                                                     x * sizeof *target_pixel;
                    // What do u mean by surface->pixels ?
  *target_pixel = pixel;
}

//majority sort : if two pictures indicate pixel[i][j] == 1, it is a face pixel
void superpostion1(SDL_Surface* image1, SDL_Surface* image2, \
        SDL_Surface* image3)
{
    SDL_LockSurface(image1);
    SDL_LockSurface(image2);
    SDL_LockSurface(image3);
    Uint32 black = 0xff;
    (void)black;// Just to avoid unused variable
    Uint32 white = 0x00;
    for (int i = 0; i<image1->w; i++)
    {
        //no need to check if all are 255 or 0 since this is after BW
        for (int j = 0; j<image1->h; j++)
        {
            Uint32 pixel1 = get_pixel(image1, i, j);
            Uint32 pixel2 = get_pixel(image2, i, j);
            Uint32 pixel3 = get_pixel(image3, i, j);
            if (pixel1== white) // or 255 if whiteblack pixel
                //info instead of binary
            {
                if (pixel2 == white || pixel3 == white)
                {
                    set_pixel(image1, i,j,white);
                }
            }
            else if (pixel2 == white && pixel3 == white)
                //or 255 if we go whiteblack)
            {
                set_pixel(image1, i,j,white);
            }
        }
    }
    SDL_UnlockSurface(image1);
    SDL_UnlockSurface(image2);
    SDL_UnlockSurface(image3);
}

//selective sort : if all 3 pictures indicate pixel[i][j] == 1,
//it is a face pixel
void superpostion2(SDL_Surface* image1, SDL_Surface* image2, \
        SDL_Surface* image3)
{
    SDL_LockSurface(image1);
    SDL_LockSurface(image2);
    SDL_LockSurface(image3);
    Uint32 black = 0xff;
    (void)black;// Just to avoid unused variable
    Uint32 white = 0x00;
    for (int i = 0; i < image1->w; i++)
    {
        for (int j = 0; j < image1->h; j++)
        {
            Uint32 pixel1 = get_pixel(image1, i, j);
            Uint32 pixel2 = get_pixel(image2, i, j);
            Uint32 pixel3 = get_pixel(image3, i, j);
            if (pixel1 == white
                && pixel2 == white
                && pixel3 ==white)
            {
                set_pixel(image1,i,j,white);
            }
        }
    }
    SDL_UnlockSurface(image1);
    SDL_UnlockSurface(image2);
    SDL_UnlockSurface(image3);
}
