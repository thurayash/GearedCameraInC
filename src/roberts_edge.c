#include "roberts_edge.h"
#include "structs.h"
#include "tools.h"


Matrix* threshold(SDL_Surface *image, Matrix* res)
{
    size_t width = image->w;
    size_t height = image->h;

    size_t high = 255 * 0.09;
    size_t low = high * 0.05;

    //Uint32 res[width][height]; On va le faire avant d'appeller la fonction
    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            if (get_pixel(image, i, j) >= high)
            {
                res->data[i][j] = 2;
            }
            if (get_pixel(image, i, j) < low)
            {
                res->data[i][j] = 0;
            }
            if (get_pixel(image, i, j) <= high && get_pixel(image, i, j) >= low)
            {
                res->data[i][j] = 1;
            }
        }
    }
    // Tu peux pas renvoyer un pointeur de pointeur d'une variable local
    // Faudrait que tu le passe en argument
    return res;
}

SDL_Surface *to_rob(SDL_Surface *image)
{
    size_t width = image->w;
    size_t height = image->h;

    for (size_t i = 0; i < width; i++) // Gray Scale
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint8 r,g,b;

            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 gray = (r+g+b)/3;

            pixel = SDL_MapRGB(image->format, gray, gray, gray);

            put_pixel(image, i, j, pixel);
        }
    }



    Uint8 r1,g1,b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
    Uint32 p1, p2, p3, p4;
    Uint32 pixel;

    for (size_t i = 0; i < width - 1; i++) // Detection
    {
        for (size_t j = 0; j < height - 1; j++)
        {
            p1 = get_pixel(image, i, j);
            p2 = get_pixel(image, i + 1, j);
            p3 = get_pixel(image, i, j + 1);
            p4 = get_pixel(image, i + 1, j + 1);

            SDL_GetRGB(p1, image->format, &r1, &g1, &b1);
            SDL_GetRGB(p2, image->format, &r2, &g2, &b2);
            SDL_GetRGB(p3, image->format, &r3, &g3, &b3);
            SDL_GetRGB(p4, image->format, &r4, &g4, &b4);

            r1 = abs(r1 - r4) + abs(r2 - r4);
            g1 = abs(g1 - g4) + abs(g2 - g4);
            b1 = abs(b1 - b4) + abs(b2 - b4);


            if( r1 > 30)// If the pixel is usefull
                pixel = SDL_MapRGB(image->format, 255, 0, 0);
            else // Useless
                pixel = SDL_MapRGB(image->format, 0, 0, 0);

            put_pixel(image, i, j, pixel);
        }
    }
    return image;
}



SDL_Surface *first_rob(SDL_Surface *image)
{
    size_t width = image->w;
    size_t height = image->h;

    for (size_t i = 0; i < width; i++) // Gray Scale
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint8 r,g,b;

            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 gray = (r+g+b)/3;

            pixel = SDL_MapRGB(image->format, gray, gray, gray);

            put_pixel(image, i, j, pixel);
        }
    }



    Uint8 r1,g1,b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
    Uint32 p1, p2, p3, p4;
    Uint32 pixel;

    for (size_t i = 0; i < width - 1; i++) // Detection
    {
        for (size_t j = 0; j < height - 1; j++)
        {
            p1 = get_pixel(image, i, j);
            p2 = get_pixel(image, i + 1, j);
            p3 = get_pixel(image, i, j + 1);
            p4 = get_pixel(image, i + 1, j + 1);

            SDL_GetRGB(p1, image->format, &r1, &g1, &b1);
            SDL_GetRGB(p2, image->format, &r2, &g2, &b2);
            SDL_GetRGB(p3, image->format, &r3, &g3, &b3);
            SDL_GetRGB(p4, image->format, &r4, &g4, &b4);

            r1 = abs(r1 - r4) + abs(r2 - r4);
            g1 = abs(g1 - g4) + abs(g2 - g4);
            b1 = abs(b1 - b4) + abs(b2 - b4);

            pixel = SDL_MapRGB(image->format, r1, g1, b1);

            put_pixel(image, i, j, pixel);
        }
    }
    return image;
}
