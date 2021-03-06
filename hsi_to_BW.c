#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>

//in case we have cb cr so that we modify rgb values directly
void hsi_to_bw(Uint8 *r, Uint8 *g, Uint8 *b,
 float *h, float *s, float *i)
{
    //in YcBcR, Y (lux) is not important, only cr and cb
    if ((*h<=179 && *s<=197 && i<=92) && 
    (*h>=0 && *s>=15 && i>=130))
    {
        *r = 255;
        *g = 255;
        *b = 255; 
    }
    else
    {
        *r = 0;
        *g = 0;
        *b = 0;
    } 
}

//in case we do not have H S I, but we have rgb
void hsi_to_bw_vrgb(Uint8 *r, Uint8 *g, Uint8 *b)
{
    if ((*r<19 && *g<95 && *b <255) && 
    (*r>6 && *g>62 && *b>227))
    {
        *r = 255;
        *g = 255;
        *b = 255; 
    }
    else
    {
        *r = 0;
        *g = 0;
        *b = 0;
    } 
       
}


int main(void)
{
    int cb =101;
    int cr = 131;

    Uint8 r =161;
    Uint8 g = 114;
    Uint8 b = 141;


    //testing
    hsi_to_bw(&r,&g,&b,&cb,&cr);
    printf("%ud ", r);

    //testing
    hsi_to_bw_vrgb(&r,&g,&b);
    printf("%ud ", r);

    return 0;
}