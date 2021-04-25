#include <math.h>
#include <stdio.h>
#include <err.h>

//#define M_PI  3.14159265359
#define MAX3(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? \
            (n) : (p)))

#define MIN(a,b) ((a) < (b)  ? (a) : (b) )
#define MIN3(a,b,c) MIN(MIN(a,b),c)

void rgb_to_hsi(float R, float G, float B, float* H, float* S, float* I, \
        float* V)
{

    float r, g, b, w, i;
    i = R+G+B;   // , min=1.e-6;
    *I = (i)/3.0; //  I: Intensity
    r=R/i;
    g=G/i;
    b=B/i;
    *V = MAX3(r,g,b);

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


void rgb_to_hsv(float R, float G, float B, float* H, float* S, float* V)
{
    float diff, cmax, cmin;
    cmax = MAX3(R,G,B);
    cmin = MIN3(R,G,B);
    diff = cmax-cmin;

    if (cmax == cmin)
        *H = 0;
    else if (cmax == R)
        *H = (int)(60 * ((G - B) / diff) + 360) % 360;
    else if (cmax == G)
        *H = (int)(60 * ((B - R) / diff) + 120) % 360;
    else if (cmax == B)
        *H = (int)(60 * ((R - G) / diff) + 240) % 360;

    if (!cmax)
        *S = 0;
    else
        *S = (diff/cmax)*100;

    *V = cmax;
}

