#include "rgb_to_hsi.h"


void rgb_to_hsi(float R, float G, float B, float* H, float* S, float* I)
{

    float r, g, b, w, i;
    i = R+G+B;   // , min=1.e-6;
    *I = (i)/3.0; //  I: Intensity
    r=R/i;
    g=G/i;
    b=B/i;

    if (R==G && G==B)
    {
        *S = 0;
        *H = 0;
    }
    else
    {
        float num = 0.5*((R-G)+(R-B));
        float denom = sqrt((R-G)*(R-G)+(R-B)*(G-B));
        w = num/denom;
        if (w>1)
            w = 1;
        if (w<-1)
            w = -1;
        *H = acos(w);   //  H: Hue
        if (*H < 0)
            err(1, "H < 0");
        if (B > G)
            *H= 2 * M_PI - *H;
        //  S: Saturation
        if (r <= g && r<= b)
            *S= 1 - 3*r;
        if (g <= r && g<= b)
            *S= 1 - 3*g;
        if (b <= r && b<= g)
            *S= 1 - 3*b;
        *S *= 100;
        *H = ((*H)*180)/M_PI;
    }
}

