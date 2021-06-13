#include "../header/circle_detection.h"


//CircleDetection Dynamic Adapt
void circleDectection_dynamicadapt(SDL_Surface *img, Candidates** arr, int R)
{
    //new matrix to fill the vote count.
    CirMatrix *matrix = new_cir_matrix(img->w, img->h,5);

    Uint8 r,g,b,r1, g1, b1,r2, g2, b2,r3, g3, b3,r4, g4, b4,r5, g5, b5,r6, g6, b6,r7, g7, b7,r8, g8, b8;

    //we start with the loswest radius
    // The width is always 752 same for the height 480
    Uint32 pix, pix1, pix2, pix3, pix4, pix5, pix6, pix7, pix8;

    for (int i = 1; i < img->w-1; i++)
    {
        for (int j = 1; j < img->h-1; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c
            pix1 = get_pixel(img, i, j+1); //from pixel.c
            pix2 = get_pixel(img, i, j-1); //from pixel.c
            pix3 = get_pixel(img, i+1, j); //from pixel.c
            pix4 = get_pixel(img, i+1, j+1); //from pixel.c
            pix5 = get_pixel(img, i+1, j-1); //from pixel.c
            pix6 = get_pixel(img, i-1, j); //from pixel.c
            pix7 = get_pixel(img, i-1, j+1); //from pixel.c
            pix8 = get_pixel(img, i-1, j-1); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            SDL_GetRGB(pix1,img->format, &r1 ,&g1, &b1);
            SDL_GetRGB(pix2,img->format, &r2 ,&g2, &b2);
            SDL_GetRGB(pix3,img->format, &r3 ,&g3, &b3);
            SDL_GetRGB(pix4,img->format, &r4 ,&g4, &b4);
            SDL_GetRGB(pix5,img->format, &r5 ,&g5, &b5);
            SDL_GetRGB(pix6,img->format, &r6 ,&g6, &b6);
            SDL_GetRGB(pix7,img->format, &r7 ,&g7, &b7);
            SDL_GetRGB(pix8,img->format, &r8 ,&g8, &b8);

            if  (  r  == 255 && g == 0
                && r1 == 255 && g1 == 0
                && r2 == 255 && g2 == 0
                && r3 == 255 && g3 == 0
                && r4 == 255 && g4 == 0
                && r5 == 255 && g5 == 0
                && r6 == 255 && g6 == 0
                && r7 == 255 && g7 == 0
                && r8 == 255 && g8 == 0
                && r8 == 255 && g8 == 0)
            {

                for (size_t theta = 0; theta < 360; theta+=2)
                {

                    int y = i + R*cos(theta);
                    int x = j + R*sin(theta);

                    if(x < 0 || x > img->w || y < 0 || y > img->h)
                        continue;

                    matrix->data[offset(x,y,0,matrix)] += 1;//maybe change y, x
                }
            }
        }
    }
    //we now have the matrix with the votes.
    //the greatest number is most likely the center

    // 3 candidates per R

    for (int i = 0; i < img->w; i++)
    {
        for (int j = 0; j < img->h; j++)
        {
            int current_val = matrix->data[offset(j,i,0,matrix)] ;

            if (current_val > arr[0]->val_max)
            {
                if (arr[2]->val_max < (int)((float)(current_val)*0.4))
                    arr[2]->x = arr[1]->x, arr[2]->y = arr[1]->y;


                if (arr[1]->val_max < (int)((float)(current_val)*0.7))
                    arr[1]->x = arr[0]->x, arr[1]->y = arr[0]->y;

                arr[0]->x = i, arr[0]->y = j;

                arr[0]->val_max = current_val;
            }
        }
    }
    free_cir_matrix(matrix);
}

