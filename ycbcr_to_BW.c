#include "ycbcr_to_BW.h"

//in case we have cb cr so that we modify rgb values directly
void ycbcr_to_bw(Uint8 *r, Uint8 *g, Uint8 *b, int *cb, int *cr)
{
    //in YcBcR, Y (lux) is not important, only cr and cb
    if ((*cb>100 && *cb<127) && (*cr>130 && *cr<175))
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

//in case we do not have CB CR, but we have rgb
void ycbcr_to_bw_vrgb(Uint8 *r, Uint8 *g, Uint8 *b)
{
    if ((*r<215 && *g<120 && *b <163) &&
    (*r>160 && *g>112 && *b>140))
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
    ycbcr_to_bw(&r,&g,&b,&cb,&cr);
    printf("%ud ", r);

    //testing
    ycbcr_to_bw_vrgb(&r,&g,&b);
    printf("%ud ", r);

    return 0;
}
