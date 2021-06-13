#include "../header/tools.h"

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
        printf("Error Code 2 : SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
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
        errx(3, "Error Code 3 :can't load %s: %s", path, IMG_GetError());

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


void draw_line(SDL_Surface* image, int x1, int y1, int x2, int y2,
        int col1, int col2, int col3)
{
    if ( (x1 >= x2 && y1 == y2) || (y1 >= y2 && x1  == x2))
        errx(EXIT_FAILURE, "Start point not compatible");
    if (x1 != x2 && y1 != y2)
        errx(EXIT_FAILURE, "Points aren't in the same line");
    if (x1 < 0 || x1 >= image->w || y1 < 0 || y1 >= image->h)
        errx(EXIT_FAILURE, "Start point outside of the image");

    if (x2 < 0 || x2 >= image->w || y2 < 0 || y2 >= image->h)
    {
        printf("X2 : %i IMGW : %i Y2 : %i IMGH : %i", \
                x2,image->w, y2, image->h);
        errx(EXIT_FAILURE, "Finish1 point outside of the image");
    }

    Uint32 pixel;
    if (x1 == x2)
        for(int j = y1; j <= y2; j++)
        {
            pixel = SDL_MapRGB(image->format, col1, col2, col3);

            put_pixel(image, x1, j, pixel);
        }
    else if (y1 == y2)
        for(int i = x1; i <= x2; i++)
        {
            pixel = SDL_MapRGB(image->format, col1, col2, col3);
            put_pixel(image, i, y1, pixel);
        }
    else
        errx(EXIT_FAILURE, "Points aren't in the same line");

    return (void)NULL;
}

int find_similarity(int *black, int *red, int size)
{
    // Index
    int b_index = 0;
    int r_index = 0;

    // Average
    int b_avg = 0;
    int r_avg = 0;

    // Find avg for each first
    for (int i = 0; i < size; i++){
        b_avg += black[i];
        r_avg += red[i];
    }
    b_avg /= size;
    r_avg /= size;

    /*if (b_avg > 115 && r_avg < 18){
        printf("256: Return b_avg:%i r_avg:%i",b_avg, r_avg );
        return -1;
    }*/

    // Compteur occurence (ta compris)
    int b_cpt = 0;
    int r_cpt = 0;

    // ?
    int b_reset = 0;
    int r_reset = 0;

    //now we get the indexes that interest us
    for (int i = 1; i < size - 1; i++)
    {
        if (black[i] <= b_avg && b_cpt <= 50)
        //maybe more or less 20 not sure yet gotta check
        //its the "length" of the face
        {
            if (black[i - 1] <= b_avg || black[i + 1] <= b_avg){
                b_index = i;
                b_reset = b_cpt;
                b_cpt++;
            }
            else
                b_cpt = 0;
        }

        if (red[i] >= r_avg/1.8 && r_cpt <= 30)
        {
            if (red[i - 1] >= r_avg || red[i + 1] >= r_avg)
            {
                r_index = i;
                r_reset = r_cpt;
                r_cpt++;
            }
            else
                r_cpt = 0;
        }
    }
    b_index -= b_reset;
    r_index -= r_reset;
    // Checker le jeu des deux index
    if ((r_index > b_index - 5 && r_index < b_index + 5)
        && (b_index > r_index - 5 && b_index < r_index + 5))
    //make it so its like within a small range
    //like 5 indexes diff possible
    {
        return b_avg;
        //return r_index;
        //return b_index;
    }
    return -1;
}

void draw_rectangle(SDL_Surface* frame, SDL_Surface* image, int x, int y,
        int size, int col1, int col2, int col3, int analyse_bool)
{
    int xA,xB, yA,yB;

    xA = x-size < 0 ? 0 : x - size;
    yA = y-size < 0 ? 0 : y - size;

    xB = x+size >= image->w  ? image->w - 2 :  x + size;

    yB = y+size >= image->h ? image->h - 2 : y + size;
    /*
    draw_line(frame, xA, yA, xB, yA, col1, col2, col3);
    draw_line(frame, xB, yA, xB, yB, col1, col2, col3);
    draw_line(frame, xA, yA, xA, yB, col1, col2, col3);
    draw_line(frame, xA, yB, xB, yB, col1, col2, col3);
    */

    int arr[4] = {xA, xB, yA, yB};
    float ratio_b, ratio_w, ratio_r = 0;
    int* histo_vert_b = calloc(size*2+1, sizeof(int));
    int* histo_hor_b = calloc(size*2+1, sizeof(int));
    int* histo_vert_r = calloc(size*2+1, sizeof(int));
    int* histo_hor_r = calloc(size*2+1, sizeof(int));


    if(analyse_bool)
        if(analyse(image, arr, &ratio_b, &ratio_w, &ratio_r, histo_vert_b,
            histo_hor_b, histo_vert_r, histo_hor_r))
            {
                //printf("=======\nblack:\n");
                //print_histo(histo_hor_b, size*2 + 1);
                //printf("red:\n");
                //print_histo(histo_hor_r, size*2 + 1);
                printf("index is: %i\n", find_similarity(histo_hor_b, histo_hor_r, size*2+1));
                if (find_similarity(histo_hor_b, histo_hor_r, size*2+1) != -1)
                {
                    draw_line(frame, xA, yA, xB, yA, col1, col2, col3);
                    draw_line(frame, xB, yA, xB, yB, col1, col2, col3);
                    draw_line(frame, xA, yA, xA, yB, col1, col2, col3);
                    draw_line(frame, xA, yB, xB, yB, col1, col2, col3);
                    printf("==========\nIS A FUCKING FACE {%f}\n=======\n", ratio_b);
                }

            }
            //printf("----\nRatio white: %f\nRatio red: %f\nRatio black: %f\n",
                    //ratio_w, ratio_r, ratio_b);

    free(histo_vert_r);
    free(histo_hor_r);
    free(histo_vert_b);
    free(histo_hor_b);

    return (void)NULL;
}

int analyse(SDL_Surface* image, int* arr, float* ratio_b,
        float* ratio_w, float* ratio_r, int* histo_vert_b, int* histo_hor_b,
        int* histo_vert_r, int* histo_hor_r)
{

    Uint32 pixel;
    Uint8 r,g,b;

    int b_p = 0, w_p = 0, r_p  = 0;

    int total = (arr[1] - arr[0])*(arr[3] - arr[2]);

    for(int i = arr[0]; i < arr[1]; i++)
    {
        for(int j = arr[2]; j < arr[3]; j++)
        {
            pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);


            if(r == 0 && g == 0){
                b_p++;
                histo_vert_b[j-arr[2]] += 1;
                histo_hor_b[i-arr[0]] += 1;
            }

            if(r == 255 && g == 255 && b == 255)
                w_p++;

            if(r == 255 && g == 0){
                r_p++;
                histo_vert_r[j-arr[2]] += 1;
                histo_hor_r[i-arr[0]] += 1;
            }
        }
    }

    *ratio_b = 100*(float)(b_p)/(float)total;
    *ratio_r = 100*(float)(r_p)/(float)total;
    *ratio_w = 100*(float)(w_p)/(float)total;

    if (b_p > (total*80)/100) // If a square is essentially black then useless
        return 0;
    return 1;
}


void print_histo(int* hist, int size)
{
    int count = 0 ;
    int prev = 0;
    printf("\n\nHistogram of pixels\n");
    for (int i = 0; i < size; i++)
    {
        count = hist[i];
        if (!count)
        {
            prev = count;
            continue;
        }
        if (!prev)
        {
            printf(".\n.\n.\n");
            prev = 1;
        }
        printf("%d |", i - 1);
        for (int j = 0; j < count / 2; j++)
        {
            printf("\u2B1B");//, (unsigned char)219);
        }
        printf(" %i\n", count);
    }
}

int save_image(SDL_Surface* img, char *path)
{
    return SDL_SaveBMP(img, path);
}

