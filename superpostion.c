#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL/SDL.h>

//majority sort : if two pictures indicate pixel[i][j] == 1, it is a face pixel
int[][] superpostion1(int[][] image1, int[][] image2, int[][] image3)
{
    int res[image1->width][image1->height]; //resultant image
    for (int i = 0; i<image1->width; i++)
    {
        for (int j = 0; j<image1->height; j++)
        {
            if (image1[i][j] == 1) // or 255 if whiteblack pixel info
            {
                if (image2[i][j] == 1 || image3[i][j] == 1)
                {
                    res[i][j] = 1;
                }
            }
            else if (image2[i][j] == 1) //or 255 if we go whiteblack)
            {
                if (image3[i][j] == 1)
                {
                    res[i][j] = 1;
                }
            }
        }
    }
    return res;
}

//selective sort : if all 3 pictures indicate pixel[i][j] == 1, it is a face pixel
int[][] superpostion2(int[][] image1, int[][] image2, int[][] image3)
{
    int res[image1->width][image1->height]; //resultant image
    for (int i = 0; i<image1->width; i++)
    {
        for (int j = 0; j<image1->height; j++)
        {
            if (image1[i][j] == 1 
                && image2[i][j] == 1 
                && image3[i][j] ==1)
            {
                res[i][j]= 1;
            }
        }
    }
    return res;
}
