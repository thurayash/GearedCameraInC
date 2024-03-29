#include <stdio.h>
#include "dilation_erosion.h"
#include "hsi_to_BW.h"
#include "superpostion.h"
#include "ycbcr_to_BW.h"
#include "tools.h"
#include "hybridation.h"
#include "circle_detection.h"

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


void Test(SDL_Surface* image, SDL_Surface* screen)
{
    /* Call here test */

    printf("IMAGE   HEIGHT : %i , WIDTH : %i\n", image->h, image->w);

    printf("Image before post-processing ...\n");

    screen = display_image(image);

    wait_for_keypressed();

    printf("Image binary ...\n");

    binary(image);

    screen = display_image(image);

    wait_for_keypressed();

    printf("Image after post-processing ...\n");

    int resx, resy;

    circleDectection_staticadapt(image, &resx, &resy);

    printf("RESX : %i , RESY : %i \n", resy, resy);

    Uint32 pixel = get_pixel(image, resy, resx);

    pixel = SDL_MapRGB(image->format, 0, 255, 0);

    put_pixel(image, resx, resy, pixel);

    screen = display_image(image);

    save_image(image, "HEY.bmp");
    wait_for_keypressed();

    (void)screen;
}


int main(int argc, char** argv)
{

    printf("TEST FILE \n");


    if (argc < 2)
    {
        printf("Not enought params : Need the image path !\n");
        return 127;
    }
    init_sdl();

    SDL_Surface* input;
    SDL_Surface* screen = NULL;

    input = load_image(argv[1]);

    Test(input, screen);

    SDL_FreeSurface(screen);
}
