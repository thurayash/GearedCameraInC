#include "tools.h"

    static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

Uint8 get_v(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 r, g, b;
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, &r, &g, &b);
    return (r + g + b)/3;

}

Uint8* get_rgb(SDL_Surface *surface, unsigned x, unsigned y)
{
    static Uint8 a[3];
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, &a[0], &a[1], &a[2]);
    return a;
}

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r,
        Uint8* g, Uint8* b)
{
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, r, g ,b);
    return ;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


SDL_Surface* new_rgb_surface(int width, int height)
{
    SDL_Surface *newSurface;

    Uint32 rmask, gmask, bmask, amask;
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    }
    else
    {
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }

    newSurface =
        SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    if (newSurface == NULL)
    {
        printf("SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        exit(1);
    }

    return newSurface;
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}



SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}



void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


void draw_line(SDL_Surface* image, int x1, int y1, int x2, int y2, int color)
{
    if ( (x1 >= x2 && y1 == y2) || (y1 >= y2 && x1  == x2))
        errx(EXIT_FAILURE, "Start point not compatible");
    if (x1 != x2 && y1 != y2)
        errx(EXIT_FAILURE, "Points aren't in the same line");
    if (x1 < 0 || x1 >= image->w || y1 < 0 || y1 >= image->h)
        errx(EXIT_FAILURE, "Start point outside of the image");

    if (x2 < 0 || x2 >= image->w || y2 < 0 || y2 >= image->h)
    {
        printf("X2 : %i IMGW : %i Y2 : %i IMGH : %i", x2,image->w, y2, image->h);
        errx(EXIT_FAILURE, "Finish1 point outside of the image");
    }

    Uint32 pixel;
    if (x1 == x2)
        for(int j = y1; j <= y2; j++)
        {
            if (color == 0)
                pixel = SDL_MapRGB(image->format, 0, 255, 0);
            else if (color == 1)
                pixel = SDL_MapRGB(image->format, 0 , 0, 255);
            else if (color == 2)
                pixel = SDL_MapRGB(image->format, 0, 255, 255);

            put_pixel(image, x1, j, pixel);
        }
    else if (y1 == y2)
        for(int i = x1; i <= x2; i++)
        {
            if (color == 0)
                pixel = SDL_MapRGB(image->format, 0, 255, 0);
            else if (color == 1)
                pixel = SDL_MapRGB(image->format, 0 , 0, 255);
            else if (color == 2)
                pixel = SDL_MapRGB(image->format, 0, 255, 255);
            put_pixel(image, i, y1, pixel);
        }
    else
        errx(EXIT_FAILURE, "Points aren't in the same line");

    return (void)NULL;
}



void draw_rectangle(SDL_Surface* image, int x, int y, int size, int color)
{
    int xA,xB, yA,yB;

    xA = x-size < 0 ? 0 : x - size;
    yA = y-size < 0 ? 0 : y - size;

    xB = x+size >= image->w  ? image->w - 2 :  x + size;

    yB = y+size >= image->h ? image->h - 2 : y + size;

    draw_line(image, xA, yA, xB, yA, color);
    draw_line(image, xB, yA, xB, yB, color);
    draw_line(image, xA, yA, xA, yB, color);
    draw_line(image, xA, yB, xB, yB, color);

    return (void)NULL;
}


int save_image(SDL_Surface* img, char *path)
{
       return SDL_SaveBMP(img, path);
}



