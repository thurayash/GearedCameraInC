#include "roberts_edge.h"

SDL_Surface *to_rob(SDL_Surface *image)
{
    size_t width = image->w;
    size_t height = image->h;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint8 r1,g1,b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
            Uint32 p1 = get_pixel(image, i, j);
            Uint32 p2 = get_pixel(image, i + 1, j);
            Uint32 p3 = get_pixel(image, i, j + 1);
            Uint32 p4 = get_pixel(image, i + 1, j + 1);

            SDL_GetRGB(p1, image->format, &r1, &g1, &b1);
            SDL_GetRGB(p2, image->format, &r2, &g2, &b2);
            SDL_GetRGB(p3, image->format, &r3, &g3, &b3);
            SDL_GetRGB(p4, image->format, &r4, &g4, &b4);
            
            r1 = abs(r1 - r4) + abs(r2 - r4);
            g1 = abs(g1 - g4) + abs(g2 - g4);
            b1 = abs(b1 - b4) + abs(b2 - b4);

            Uint32 pixel = SDL_MapRGB(image->format, r1, g1, b1);

            put_pixel(image, i, j, pixel);
        }
    }
    return image;
}
