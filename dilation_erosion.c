#include <dilation_erosion.h> // Avoid including things outside the header

// Best dilate by one solution// without Manhattan Oracle solution
void dilate(SDL_Surface* image)
{
    SDL_LockSurface(image);
    Uint8 r,g,b;

    for (size_t i=0; i < image->w; i++)
    {
        for (size_t j=0; j< image->h; j++)
        {
            Uint32 pixel = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pixel,image->format, &r ,&g, &b);
            if (r != 0) //==1 in binary
            {
                if (i>0 && get_pixel(image, (i-1), j)==0)
                    put_pixel(image, (i-1), j,pixel);

                if (j>0 && get_pixel(image, i, (j-1))==0)
                    put_pixel(image, i, (j-1),pixel);

                if (i+1<image->w && get_pixel(image, (i+1), j)==0)
                    put_pixel(image, (i+1), j,pixel);

                if (j+1<image[i]->length && get_pixel(image, i, (j+1))==0) // I don't get it ?! What do u mean by image[i]->length ?
                    put_pixel(image, i, (j+1),pixel);
            }
        }
    }
    SDL_UnlockSurface(image);
}

//erode is the "dilation" of the background. it should override dilation
//so you always dilate, then erode
void erode(SDL_Surface *image)
{
    SDL_LockSurface(image);
    Uint8 r,g,b;
    for (size_t i=0; i < image->w; i++)
    {
        for (size_t j=0; j< image->h; j++) // U can avoid writing this numbers of if by else if, it's more opti
        { // It may be better to create a const for the image w and h
            Uint32 pixel = get_pixel(image, i, j); //from pixel.c //to be able to easily manipulate pixels
            SDL_GetRGB(pixel,image->format, &r ,&g, &b);
            if (r == 0)
            {
                if (i>0 && get_pixel(image, (i-1), j)==0)
                    put_pixel_black(image, (i-1), j,pixel);

                if (j>0 && get_pixel(image, i, (j-1))==0)
                    put_pixel_black(image, i, (j-1),pixel);

                if (i+1<image->w && get_pixel(image, (i+1), j)==0)
                    put_pixel_black(image, (i+1), j,pixel);

                if (j+1<image[i]->length && get_pixel(image, i, (j+1))==0)// I don't get it ?! What do u mean by image[i]->length ?
                    put_pixel_black(image, i, (j+1),pixel);
            }
        }
    }
}
