#include <stdio.h>
#include "dilation_erosion.h"
#include "hsi_to_BW.h"
#include "superpostion.h"
#include "ycbcr_to_BW.h"
#include "tools.h"
#include "hybridation.h"


void Test(SDL_Surface* image, SDL_Surface* screen)
{
    /* Call here test */


    printf("Image before post-processing ...\n");

    screen = display_image(image);

    wait_for_keypressed();

    printf("Image after post-processing ...\n");

    image_conversion(image);

    screen = display_image(image);

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
