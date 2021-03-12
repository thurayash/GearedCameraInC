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

    for (size_t i = 0; i < width; i++)
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

    /*SDL_Surface *result = new_rgb_surface(width, height);


    for (size_t i = 1; i < width - 1; i++)
    {
        for (size_t j = 1; j < height - 1; j++)
        {

            Uint32 white = SDL_MapRGB(image->format, 0, 0, 0);

            //Uint8 r1,g1,b1, r2, g2, b2, r3, g3, b3, r4, g4, b4, r5, g5, b5, r6, g6, b6, r7, g7, b7, r8, g8, b8, r9, g9, b9;
            Uint32 p1 = get_pixel(image, i - 1, j - 1);
            Uint32 p2 = get_pixel(image, i, j - 1);
            Uint32 p3 = get_pixel(image, i + 1, j - 1);
            Uint32 p4 = get_pixel(image, i - 1, j);
            Uint32 p5 = get_pixel(image, i, j);
            Uint32 p6 = get_pixel(image, i + 1, j);
            Uint32 p7 = get_pixel(image, i - 1, j + 1);
            Uint32 p8 = get_pixel(image, i, j + 1);
            Uint32 p9 = get_pixel(image, i + 1, j + 1);

            if (p1 == white || p2 == white || p3 == white|| \
                p4 == white || p6 == white ||  \
                p7 == white || p8 == white || p9 == white)
            {
                Uint32 pixel = SDL_MapRGB(result->format, 255 ,0,0);
                put_pixel(result, i, j, pixel);
            }

        }
    }
    //SDL_SaveBMP(result, "test.bmp");*/
    Matrix* res = new_matrix(height, width);

    threshold(image, res);


    SDL_Surface *result = new_rgb_surface(width, height);

    for (size_t i = 1; i < width - 1; i++)
    {
        for (size_t j = 1; j < height - 1; j++)
        {
            Uint32 p1 = res->data[i - 1][j - 1];
            Uint32 p2 = res->data[i][j - 1];
            Uint32 p3 = res->data[i + 1][j - 1];
            Uint32 p4 = res->data[i - 1][j];
            //Uint32 p5 = res[i][j];
            Uint32 p6 = res->data[i + 1][j];
            Uint32 p7 = res->data[i - 1][j + 1];
            Uint32 p8 = res->data[i][j + 1];
            Uint32 p9 = res->data[i + 1][j + 1];

            if (p1 == 2 || p2 == 2 || p3 == 2|| \
                p4 == 2 || p6 == 2 ||  \
                p7 == 2 || p8 == 2 || p9 == 2)
            {
                Uint32 pixel = SDL_MapRGB(result->format, 255 ,0,0);
                put_pixel(result, i, j, pixel);
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(result->format, 0, 0, 0);
                put_pixel(result, i, j, pixel);
            }
        }
    }
    return image;
}
