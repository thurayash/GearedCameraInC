#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <pixel.h>
#include <dilation_erosion.h>

// Dilate : Cross form 3x3
SDL_Surface dilate_cross(SDL_Surface image)
{
    SDL_Surface result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;

    for (size_t i=2; i < (image->w - 2); i++)
    {
        for (size_t j=2; j< (image->h - 2); j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 255)
            {
               put_pixel(result, i, j,0x00);
               put_pixel(result, (i-2), j,0x00);
               put_pixel(result, (i-1), j,0x00);
               put_pixel(result, i, (j-2),0x00);
               put_pixel(result, i, (j-1),0x00);
               put_pixel(result, (i+2), (j),0x00);
               put_pixel(result, (i+1), j,0x00);
               put_pixel(result, i, (j+2),0x00);
               put_pixel(result, i, (j+1),0x00);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    SDL_FreeSurface(image);
    return result;
}

//erosion 3sqaures cross
SDL_Surface erode_cross(SDL_Surface image)
{
    SDL_Surface result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;
    for (size_t i=0; i < image->w; i++)
    {
        for (size_t j=0; j< image->h; j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 0)
            {
                put_pixel(result, i, j,0xff);
                put_pixel(result, (i-2), j,0xff);
                put_pixel(result, (i-1), j,0xff);
                put_pixel(result, i, (j-2),0xff);
                put_pixel(result, i, (j-1),0xff);
                put_pixel(result, (i+2), (j),0xff);
                put_pixel(result, (i+1), j,0xff);
                put_pixel(result, i, (j+2),0xff);
                put_pixel(result, i, (j+1),0xff);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    SDL_FreeSurface(image);
    return result;
}

//3x3 square dilate
SDL_Surface dilate_square(SDL_Surface image)
{
    SDL_Surface result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;

    for (size_t i=2; i < (image->w - 2); i++)
    {
        for (size_t j=2; j< (image->h - 2); j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 255)
            {
                put_pixel(result, i, j,0x00);
                put_pixel(result, (i-2), j,0x00);
                put_pixel(result, (i-1), j,0x00);
                put_pixel(result, i, (j-2),0x00);
                put_pixel(result, i, (j-1),0x00);
                put_pixel(result, (i+2), (j),0x00);
                put_pixel(result, (i+1), j,0x00);
                put_pixel(result, i, (j+2),0x00);
                put_pixel(result, i, (j+1),0x00);
                put_pixel(result, (i+1), (j+1),0x00);
                put_pixel(result, (i-1), (j+1),0x00);
                put_pixel(result, (i+1), (j-1),0x00);
                put_pixel(result, (i-1), (j-1),0x00);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    SDL_FreeSurface(image);
    return result;
}

//erosion 3x3 square
SDL_Surface erode_square(SDL_Surface image)
{
    SDL_Surface result = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(result);
    Uint8 r,g,b;
    for (size_t i=0; i < image->w; i++)
    {
        for (size_t j=0; j< image->h; j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 0)
            {
                put_pixel(result, i, j,0xff);
                put_pixel(result, (i-2), j,0xff);
                put_pixel(result, (i-1), j,0xff);
                put_pixel(result, i, (j-2),0xff);
                put_pixel(result, i, (j-1),0xff);
                put_pixel(result, (i+2), (j),0xff);
                put_pixel(result, (i+1), j,0xff);
                put_pixel(result, i, (j+2),0xff);
                put_pixel(result, i, (j+1),0xff);
                put_pixel(result, (i+1), (j+1),0xff);
                put_pixel(result, (i-1), (j+1),0xff);
                put_pixel(result, (i+1), (j-1),0xff);
                put_pixel(result, (i-1), (j-1),0xff);
            }
        }
    }
    SDL_UnlockSurface(image);
    SDL_UnlockSurface(result);
    SDL_FreeSurface(image);
    return result;
}

