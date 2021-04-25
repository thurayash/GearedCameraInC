#include "dilation_erosion.h"

void sett_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    //might need to make surface->pixel into Uint8 *
  Uint32 *target_pixel =  surface->pixels + y * surface->pitch +
                                                     x * sizeof *target_pixel;
                    // What do u mean by surface->pixels ?
  *target_pixel = pixel;
}

// Dilate : Cross form 3x3
SDL_Surface* dilate_cross(SDL_Surface* image)
{
    SDL_Surface* result = \
            SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    // Humm, if we give it as param it would take less time to compute
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;

    for (int i=2; i < (image->w - 2); i++)
    {
        for (int j=2; j< (image->h - 2); j++)
        {
            Uint32 uull = get_pixel(image, i, j);
            //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 255)
                // It's not auto. white it can be gray unless u work
            {
               sett_pixel(result, i, j,0x00);
               sett_pixel(result, (i-2), j,0x00);
               sett_pixel(result, (i-1), j,0x00);
               sett_pixel(result, i, (j-2),0x00);
               sett_pixel(result, i, (j-1),0x00);
               sett_pixel(result, (i+2), (j),0x00);
               sett_pixel(result, (i+1), j,0x00);
               sett_pixel(result, i, (j+2),0x00);
               sett_pixel(result, i, (j+1),0x00);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    return result;
}

//erosion 3sqaures cross
SDL_Surface* erode_cross(SDL_Surface* image)
{
    SDL_Surface* result = \
            SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;
    for (int i=0; i < image->w; i++)
    {
        for (int j=0; j< image->h; j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 0)// Same thing
            {
                sett_pixel(result, i, j,0xff);
                sett_pixel(result, (i-2), j,0xff);
                sett_pixel(result, (i-1), j,0xff);
                sett_pixel(result, i, (j-2),0xff);
                sett_pixel(result, i, (j-1),0xff);
                sett_pixel(result, (i+2), (j),0xff);
                sett_pixel(result, (i+1), j,0xff);
                sett_pixel(result, i, (j+2),0xff);
                sett_pixel(result, i, (j+1),0xff);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    return result;
}
//3x3 square dilate
void dilate_square(SDL_Surface* image, SDL_Surface* result)
{
    Uint8 r,g,b;

    for (int i=2; i < (result->w - 2); i++)
    {
        for (int j=2; j< (result->h - 2); j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(uull,result->format, &r ,&g, &b);
            if (r == 255)// It's not auto. white it can be gray unless u work
                // On a binary image
            {
                uull = SDL_MapRGB(result->format, 255, 255, 255);
                put_pixel(result, i, j, uull);
                put_pixel(result, i-2, j, uull);
                put_pixel(result, i-1, j, uull);
                put_pixel(result, i, j-2, uull);
                put_pixel(result, i, j-1, uull);
                put_pixel(result, i+2, j, uull);
                put_pixel(result, i+1, j, uull);
                put_pixel(result, i, j+2, uull);
                put_pixel(result, i, j+1, uull);
                put_pixel(result, i+1, j+1, uull);
                put_pixel(result, i-1, j+1, uull);
                put_pixel(result, i+1, j-1, uull);
                put_pixel(result, i-1, j-1, uull);
            }
        }
    }
}

//erosion 3x3 square
void erode_square(SDL_Surface* image, SDL_Surface* result)
{
    Uint8 r,g,b;
    for (int i= 2; i < image->w - 2; i++)
    {
        for (int j= 2; j< image->h - 2; j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 0) // Same thing, it can be gray
            {
                uull = SDL_MapRGB(result->format, 255, 255, 255);
                put_pixel(result, i, j, uull);
                put_pixel(result, i-2, j, uull);
                put_pixel(result, i-1, j, uull);
                put_pixel(result, i, j-2, uull);
                put_pixel(result, i, j-1, uull);
                put_pixel(result, i+2, j, uull);
                put_pixel(result, i+1, j, uull);
                put_pixel(result, i, j+2, uull);
                put_pixel(result, i, j+1, uull);
                put_pixel(result, i+1, j+1, uull);
                put_pixel(result, i-1, j+1, uull);
                put_pixel(result, i+1, j-1, uull);
                put_pixel(result, i-1, j-1, uull);
            }
        }
    }
    for(int i = 0; i < result->w; i++)
    {
        for(int j =0; j < result->h; j++)
        {
            Uint32 uull = get_pixel(result, i, j);
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if(r == 255) // White pixel
            {
                uull = SDL_MapRGB(result->format, 0,0,0);
                put_pixel(result, i,j,uull);
            }
            else
            {
                uull = SDL_MapRGB(result->format, 255,255,255);
                put_pixel(result, i, j, uull);
            }
        }
    }
}

