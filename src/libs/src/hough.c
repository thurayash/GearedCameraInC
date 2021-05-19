#include "../header/hough.h"
void binary(SDL_Surface* image)
{
    int height = image->h;
    int width  = image->w;

    Uint32 pixel;
    Uint8 r, g, b;

    printf("IMAGE_BINARY   HEIGHT : %i , WIDTH : %i\n", height, width);
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            pixel = get_pixel(image, i, j);

            SDL_GetRGB(pixel, image->format, &r , &g, &b);

            if(r > 126)
                pixel = SDL_MapRGB(image->format, 255, 255, 255);
            else
                pixel = SDL_MapRGB(image->format, 0, 0, 0);
            put_pixel(image, i,j, pixel);
        }
    }

}
