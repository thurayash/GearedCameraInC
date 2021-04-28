#include "image_conversion.h"

void to_ycbcr(int r, int g, int b, float *y, float *cb, float *cr)
{
    *y = 16 + (0.2567890625*r) + (0.50412890625)*g + 0.09790625*b;
    *cb = 128 - 0.14822265625*r - 0.2909921875*g + 0.43921484375*b;
    *cr = 128 + 0.43921484375 * r - 0.3677890625*g - 0.07142578125 * b;
}

void image_conversion(SDL_Surface* image, SDL_Surface* skin)
{
    size_t width = image->w;
    size_t height = image->h;

    for(size_t i = 0; i < width; i++)
    {
        for(size_t j = 0; j < height; j++)
        {
            Uint8 r,g,b;
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            float R_p, G_p, B_p;
            R_p = (float)r/(float)(r+g+b);
            G_p = (float)g/(float)(r+g+b);
            B_p = (float)b/(float)(r+g+b);

            float H,S,V;
            V = MAX3(R_p, G_p, B_p);

            if (V != 0)
                S = (V- MIN3(R_p, G_p, B_p))/V;
            else
                S = 0;

            if (V == R_p)
                H = 60*(G_p-B_p)/(V-MIN3(R_p, G_p, B_p));
            else if (V == G_p)
                H = 2 + 60*(B_p - R_p)/(V - MIN3(R_p, G_p, B_p));
            else if (V == B_p)
                H = 4 + 60*(R_p - G_p)/(V - MIN3(R_p, G_p, B_p));

            if (H < 0)
                H = H + 360;

            float Y,Cb,Cr;
            to_ycbcr(r,g,b,&Y,&Cb,&Cr);


            if (R_p/G_p > 1.185 && (0.2 <= S && S <= 0.6) && \
                    ((0 <= H && H <= 25) || (335 <= H && H <= 360)) && \
                    (77 < Cb && Cb < 127) && (133 < Cr && Cr < 173))
                pixel = SDL_MapRGB(image->format, 255 ,255,255);
            else
                pixel = SDL_MapRGB(image->format, 0, 0, 0);

            put_pixel(skin, i, j, pixel);

        }
    }

}
