#include "../header/roberts_edge.h"
#include "../header/structs.h"
#include "../header/tools.h"


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
            if (get_pixel(image, i, j) <= high && \
                    get_pixel(image, i, j) >= low)
            {
                res->data[i][j] = 1;
            }
        }
    }
    // Tu peux pas renvoyer un pointeur de pointeur d'une variable local
    // Faudrait que tu le passe en argument
    return res;
}

void to_rob(SDL_Surface *image, int threshold)
{
    size_t width = image->w;
    size_t height = image->h;

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


            if (r1 > threshold)
                r1 = 0;
            b1 = g1 = r1;

            if (r2 > threshold)
                r2 = 0;
            b2 = g2 = r2;

            if (r3 > threshold)
                r3 = 0;
            b3 = g3 = r3;

            if (r4 > threshold)
                r4 = 0;
            b4 = g4 = r4;

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

    return (void)NULL;
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



Uint8 Threshold_value(int width, int height, unsigned long *histo)
{
    Uint8 threshold =  0;
    //Uint8 threshold2 = 0;
    double nbrPixel = width * height;

    unsigned long sum = 0;
    unsigned long w1 = 0,w2 = 0;
    unsigned long m1 = 0,m2 = 0;
    float valence = 0.0f;
    unsigned long sumB = 0;
    unsigned long var_max = 0;

    for(int i = 0; i < 256; i++){
        sum +=  i*histo[i];
    }

    for(int t = 0; t < 256; t++)
    {
        w1 += histo[t];
        if (w1 == 0)
            continue;
        if (w1 >= nbrPixel)
            break;
        w2 = nbrPixel - w1;


        sumB += t*histo[t];
        m1 = sumB/w1;
        m2 = (sum-sumB)/w2;

        valence = w1*w2*(m1-m2)*(m1-m2);

        if (valence >= var_max)
        {
            threshold = t;
            if(valence > var_max)
                //threshold2 = t;
                var_max = valence;
        }
    }
    return threshold;
}


