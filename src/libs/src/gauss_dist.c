#include "../header/gauss_dist.h"
//id == which Circle detection radius, as in which distance camera-face
//coord of the two centers
int gauss_dist(int id, int x1, int y1, int x2, int y2)
{
    //here we need to measure how many pixels is in an average head on each
    //distance
    //of every Circle detection radius

    //int list[] = {(radius of detection, length in pixel of a face) * number
    //of how many circle radii};
    int xres = abs(x1 - x2);
    int yres = abs(y1 - y2);
    int pixdist = fmax(xres, yres);
    //25 cm is average face (check value!!!)
    //list[id][1] == the relative length of the face infered from the distance
    //of circle detection
    //converted to pixel length
    return (pixdist * 25) / 70; //list[id][1];
}

//x1 y 1 center coord of first frame
//x2 y 2 center coord of second frame
//dist1 is the distance between camera and first face
//dist 2 is that of second face
//both dist1 and 2 are inferred from the Circle detection algo ( which colored
//rectangle)
double* find_angle(int x1, int y1, int x2, int y2, int dist1, int dist2)
{
    int Xgdist = gauss_dist(dist1, x1, y1, x2, y2);
    double dist22 = sqrt((double)((Xgdist*Xgdist) + (dist1*dist1)));
    double alpha = cos(((double)dist1)/dist22);
    //big gear angle
    double biggear = acos(alpha);
    //check if it is y1 x2 for the second point
    //do we have :?
    //  ---------->y
    //  |
    //  | x
    int Xgdist2 = gauss_dist(dist1,x2,y2, x1,y2);
    double beta = Xgdist/ dist22;
    double smallgear = asin(beta);
    double *result = malloc(sizeof(double) * 2);
    result[0] = biggear;
    result[1] = smallgear;

    return result;
}
