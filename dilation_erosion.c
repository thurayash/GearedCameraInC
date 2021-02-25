#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>
#include <pixel.h>


// Best dilate by one solution// without Manhattan Oracle solution 
void dilate(SDL_Surface* image)
{
    SDL_LockSurface(image);
    Uint8 r,g,b;

    for (size_t i=0; i < image->width; i++)
    {
        for (size_t j=0; j< image->height; j++)
        {
            Uint32 pixel = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pixel,image->format, &r ,&g, &b);
            if (r != 0) //==1 in binary
            {
                if (i>0 && get_pixel(image, (i-1), j)==0)
                    put_pixel(*image, (i-1), j,pixel);

                if (j>0 && get_pixel(image, i, (j-1))==0)
                    put_pixel(*image, i, (j-1),pixel);

                if (i+1<image->width && get_pixel(image, (i+1), j)==0)
                    put_pixel(*image, (i+1), j,pixel);

                if (j+1<image[i]->length && get_pixel(image, i, (j+1))==0)
                    put_pixel(*image, i, (j+1),pixel);
            }
        }
    }
    SDL_UnlockSurface(image);
}

//erode is the "dilation" of the background. it should override dilation
//so you always dilate, then erode
void erode(SDL_Surface *image)
{
    SDL_LockSurface(*image);
    Uint8 r,g,b;
    for (size_t i=0; i < image->width; i++)
    {
        for (size_t j=0; j< image->height; j++)
        {
            Uint32 pixel = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pixel,image->format, &r ,&g, &b);
            if (r == 0)
            {
                if (i>0 && get_pixel(image, (i-1), j)==0)
                    put_pixel_black(*image, (i-1), j,pixel);

                if (j>0 && get_pixel(image, i, (j-1))==0)
                    put_pixel_black(*image, i, (j-1),pixel);

                if (i+1<image->width && get_pixel(image, (i+1), j)==0)
                    put_pixel_black(*image, (i+1), j,pixel);

                if (j+1<image[i]->length && get_pixel(image, i, (j+1))==0)
                    put_pixel_black(*image, i, (j+1),pixel);
            }
        }
    }
}
