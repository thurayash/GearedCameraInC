#include "circle_detection.h"
//pass in the "containers" for the center pixel coordinates

//R 127
void circleDectection(SDL_Surface *img, int *resx, int *resy)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);

    Uint8 r,g,b;

    int R = 127; //this is the radius of an average face in pixel, might need to change this

    int counterx; //to be able to detect when we are inside of an already
    int countery; // drawn circle. we do not want over superpostion

    //the minus three is a safetynet from circle drawing pixel shenanigins
    for (int i = R+3; i < img->w-R-3; i++)
    {
        for (int j = R+3; j < img->h-R-3; j++)
        {
            Uint32 pix = get_pixel(img, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255) //if it is white // face edge pixel (it is in binary since BW)
            {
                if ((counterx >= i+R-R/10||counterx <= i-R + R/10)||(countery >= j+R -R/10|| countery <= j-R + R/10))
                {
                    counterx =i;
                    countery = j;
                    //we draw the circle, taking the center the pixel we found
                    //theta is the 360 circle, though we might need to skip
                    //with cos and sin some theta values
                    for (size_t theta = 0; theta < 360; theta+=2)
                    {
                        int x = i + R*cos(theta); //cos takes double, check for this
                        int y = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }
            }

        }
    }

    //we now have the matrix with the votes. the greatest number is most likely the center
    //if more have the same value, FOR NOW we take the first we find
    int votes =0;
    for (int v = 0; v < img->w; v++)
    {
        for (int b = 0; b < img->h; b++)
        {
            //to make votes change position with equal votes in m->data, add <=
            if (votes < matrix->data[v][b])
            {
                votes = matrix->data[v][b];
                *resx = v;
                *resy = b;
            }

        }

    }
}



//static adapt makes the Radius of the circle into image width/4
//CircleDetection Static Adapt
void circleDectection_staticadapt(SDL_Surface *img, int *resx, int *resy)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);

    Uint8 r,g,b;

    int R = img->h /4; //this is the radius of an average face in pixel, might need to change this
    // The width is always 752 same for the height 480
    int counterx, countery;
    Uint32 pix;
    for (int i = R+3; i < img->w -R -3; i++)
    {
        for (int j = R+3; j < img->h -R -3; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255 ) //if it is white // face edge pixel (it is in binary since BW)
            {

                if ((counterx >= i+R-R/10||counterx <= i-R + R/10) || \
                        (countery >= j+R -R/10|| countery <= j-R + R/10))
                {
                    counterx =i;
                    countery = j;
                    //we draw the circle, taking the center the pixel we found
                    //theta is the 360 circle, though we might need to skip
                    //with cos and sin some theta values
                    for (size_t theta = 0; theta < 360; theta+=2)
                    {
                        int y = i + R*cos(theta); //cos takes double, check for this
                        int x = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }
            }
        }
    }
    //we now have the matrix with the votes. the greatest number is most likely the center
    //if more have the same value, FOR NOW we take the first we find
    int votes =0;
    for (int v = 0; v < matrix->cols; v++)
    {
        for (int b = 0; b < matrix->rows; b++)
        {
            //if (counterx == 266 && countery == 163 && i == 507 && j == 148 && R == 145)
            //{
            //    printf("Done\n");
            //}

            //to make votes change position with equal votes in m->data, add <=
            if (votes < matrix->data[b][v])
            {
                votes = matrix->data[b][v];
                *resx = v;
                *resy = b;
            }

        }

    }
}


// CircleDetection Dynamic
void circleDectection_dynamic(SDL_Surface *img, int *resx, int *resy)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);
    //make a list of matrices, then for each have a certain R value in the theta loop
    Uint8 r,g,b;

    int R = 50; //this is the radius of an average face in pixel, might need to change this
    int counterx =0;
    int countery =0; //to know if we are in a circle already drawnm so as not to take it
    int i =0;
    int j=0;
    for (i = R+3; i < img->w - R -3; i++)
    {
        for (j = R+3; j < img->h -R-3; j++)
        {
            Uint32 pix = get_pixel(img, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255) //if it is white // face edge pixel (it is in binary since BW)
            {
                if ((counterx >= i+R-R/10||counterx <= i-R + R/10)||(countery >= j+R -R/10|| countery <= j-R + R/10))
                {
                    counterx =i;
                    countery = j;
                    //we draw the circle, taking the center the pixel we found
                    //theta is the 360 circle, though we might need to skip
                    //with cos and sin some theta values
                    for (size_t theta = 0; theta < 360; theta+=2)
                    {
                        int x = i + R*cos(theta); //cos takes double, check for this
                        int y = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }

            }

        }

    }

    //we now have the matrix with the votes. the greatest number is most likely the center
    //if more have the same value, FOR NOW we take the first we find
    int votes =0;
    for (int v = 0; v < img->w; v++)
    {
        for (int b = 0; b < img->h; b++)
        {
            //to make votes change position with equal votes in m->data, add <=
            if (votes < matrix->data[v][b])
            {
                votes = matrix->data[v][b];
                *resx = v;
                *resy = b;
            }

        }

    }
}
