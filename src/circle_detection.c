#include "circle_detection.h"

//res1 is the new center detected //res2 is the previous frame center
//return 1 if center should change //return 0 else
//you change resx1 to resx2, otherwise you will stay stuck
int center_block(int *resx1, int *resy1, int *resx2, int *resy2)
{
    //the center block dimensions wille be 50 x 50 pixels for now
    if (resx1 > resx2 + 50 || resx1 < resx2 + 50)
    {
        return 1;
    }
    else if (resy1 > resy2 + 50 || resy1 < resy2 + 50)
    {
        return 1;
    }
    resx2 = resx1;
    resy2 = resy1;
    return 0;
}


//static adapt makes the Radius of the circle into image width/4
//CircleDetection Static Adapt
void circleDectection_staticadapt(SDL_Surface *img, int *resx, int *resy)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);

    Uint8 r,g,b;

    int R = img->h /4; //this is the radius of an average face in pixel,
    // might need to change this
    // The width is always 752 same for the height 480
    int counterx, countery;
    Uint32 pix;
    for (int i = R+3; i < img->w -R -3; i++)
    {
        for (int j = R+3; j < img->h -R -3; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255 ) //if it is white
                // face edge pixel (it is in binary since BW)
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
                        int y = i + R*cos(theta);
                        //cos takes double, check for this
                        int x = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }
            }
        }
    }
    //we now have the matrix with the votes.
    //the greatest number is most likely the center
    int votes =0;
    for (int v = 0; v < img->w; v++)
    {
        for (int b = 0; b < img->h; b++)
        {
            //to make votes change position with equal votes in m->data, add <=
            if (votes < matrix->data[b][v])
            {
                votes = matrix->data[b][v];
                *resx = v;
                *resy = b;
            }

        }
    }
    free_matrix(matrix);
}

//CircleDetection Static Adapt with center block
void circleDectection2_staticadapt(SDL_Surface *img, int *cx, int *cy)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);

    Uint8 r,g,b;

    int *resx = NULL;
    int *resy = NULL;

    int R = img->h /4; //this is the radius of an average face in pixel,
    // might need to change this
    // The width is always 752 same for the height 480
    int counterx, countery;
    Uint32 pix;
    for (int i = R+3; i < img->w -R -3; i++)
    {
        for (int j = R+3; j < img->h -R -3; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255 ) //if it is white
                // face edge pixel (it is in binary since BW)
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
                        int y = i + R*cos(theta);
                        //cos takes double, check for this
                        int x = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }
            }
        }
    }
    //we now have the matrix with the votes.
    //the greatest number is most likely the center
    //if more have the same value, FOR NOW we take the first we find
    int votes =0;
    for (int v = 0; v < matrix->cols; v++)
    {
        for (int b = 0; b < matrix->rows; b++)
        {
            //to make votes change position with equal votes in m->data, add <=
            if (votes < matrix->data[b][v])
            {
                votes = matrix->data[b][v];
                *resx = v;
                *resy = b;
            }

        }
    }
    //we dont need to always return the exact center
    //we need a stable camera, so if the exact center is still
    //inside the cetner block, we do nothing
    if (center_block(resx,resy, cx , cy))
    {
        printf("%s%ls - %ls\n", "center changed : ",resx,resy);
    }
    free_matrix(matrix);
}

//this circle detection return an array with the three top coordinates
//for centers
//arr and arr+1 are the first x y ; then arr+2 arr+3 ; then arr+4 arr+5
//for the third point
//int arr[6] = {0,0,0,0,0,0}; or *arr should be fed into as arguments
void circleDectection3_staticadapt(SDL_Surface *img, int *arr)
{
    //new matrix to fill the vote count.
    Matrix *matrix = new_matrix(img->w, img->h);

    Uint8 r,g,b;

    int R = img->h /4; //this is the radius of an average face in pixel,
    // might need to change this
    // The width is always 752 same for the height 480
    int counterx, countery;
    Uint32 pix;
    for (int i = R+3; i < img->w -R -3; i++)
    {
        for (int j = R+3; j < img->h -R -3; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255 ) //if it is white
                // face edge pixel (it is in binary since BW)
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
                        int y = i + R*cos(theta);
                        //cos takes double, check for this
                        int x = j + R*sin(theta); //sin takes double as well
                        matrix->data[x][y] +=1;
                    }
                }
            }
        }
    }
    //we now have the matrix with the votes.
    //the greatest number is most likely the center
    int vote1 =0;
    int vote2 =0;
    int vote3 =0;
    for (int v = 0; v < matrix->cols; v++)
    {
        for (int b = 0; b < matrix->rows; b++)
        {

            /* If current element is greater than first*/
            if (matrix->data[b][v] > vote1)
            {
                vote3 = vote2;
                vote2 = vote1;
                vote1 = matrix->data[b][v];
                *arr = v;
                *(arr +1) = b;
            }

            /* If arr[i] is in between first and second then update second  */
            else if (matrix->data[b][v] > vote2)
            {
                vote3 = vote2;
                vote2 = matrix->data[b][v];
                *(arr+2) = v;
                *(arr +3) = b;
            }

            else if (matrix->data[b][v] > vote3)
            {
                vote3 = matrix->data[b][v];
                *(arr+4) = v;
                *(arr +5) = b;
            }
        }
    }
    free_matrix(matrix);
}

//CircleDetection Dynamic Adapt
void circleDectection_dynamicadapt(SDL_Surface *img, int *arr, int radinc)
{
    //new matrix to fill the vote count.
    CirMatrix *matrix = new_cir_matrix(img->w, img->h,5);

    Uint8 r,g,b;

    //we start with the loswest radius
    int R = img->h /12;
    // The width is always 752 same for the height 480
    int counterx, countery;
    Uint32 pix;
    for (int i = R+3; i < img->w -R -3; i++)
    {
        for (int j = R+3; j < img->h -R -3; j++)
        {
            pix = get_pixel(img, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(pix,img->format, &r ,&g, &b);
            if (r == 255 ) //if it is white
                // face edge pixel (it is in binary since BW)
            {

                //here is where we will need to check for each Radius
                //if there was a circle already drawn to not take the point
                for (int radin = 0; radin <= radinc; radin++)
                {
                    R+= radin;
                    if ((counterx >= i+R-R/10||counterx <= i-R + R/10) || \
                            (countery >= j+R -R/10|| countery <= j-R + R/10))
                    {
                        counterx =i;
                        countery = j;
                        for (size_t theta = 0; theta < 360; theta+=2)
                        {
                            int y = i + R*cos(theta);
                            //cos takes double, check for this
                            int x = j + R*sin(theta); //sin takes double as
                            //well

                            //maybe change y, x
                            matrix->data[offset(x,y,radin,matrix)] +=1;
                        }
                    }
                    R-=radin;
                }
            }
        }
    }
    //we now have the matrix with the votes.
    //the greatest number is most likely the center
    int *votes = calloc(radinc ,sizeof(int));//maybe =NULL
    for (int v = 0; v < img->w; v++)
    {
        for (int b = 0; b < img->h; b++)
        {
            for (int rad = 0; rad <= radinc; rad++)
            {
                if (*(votes+rad) < matrix->data[offset(b,v,rad,matrix)])
                {
                    //ar
                    *(votes+rad) = matrix->data[offset(b,v,rad, matrix)];
                    *(arr+rad)= v;
                    *(arr + 1 + rad) = b;
                }
            }
        }
    }
    free(votes); //maybe?
    free_cir_matrix(matrix);
}
