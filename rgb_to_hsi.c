#include <math.h>
#include <stdio.h>
#include <err.h>

// #define M_PI  3.14159265359

void rgb_to_hsi(double R, double G, double B, double* H, double* S, double* I)
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
        double num = 0.5*((R-G)+(R-B));
        double denom = sqrt((R-G)*(R-G)+(R-B)*(G-B));
        w = num/denom;
        if (w > 1)
            w = 1;
        if (w < -1)
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

int main(void)
{
    double S;
    double H;
    double I;

    double R = 244;
    double G = 89;
    double B = 167;

    rgb_to_hsi(R,G,B, &H, &S, &I);

    printf("H: %f\nS: %f\nI: %f\n", H, S, I);

    return 0;
}
