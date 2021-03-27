/*
 * Made with Hearth - Yassine
 *
 * Make sure to compile the .c file using these flags:
 *
 *          gcc -g vidtest.c -std=gnu99 -lSDL -lSDL_image -O3 -O0 -o wc
 *
*/

#include "vidtest.h"
#include "rgb_to_hsi.h"
#include "ycbcr.h"
#include <math.h>
#include "roberts_edge.h"

#define MAX3(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? \
            (n) : (p)))

#define MIN(a,b) ((a) < (b)  ? (a) : (b) )
#define MIN3(a,b,c) MIN(MIN(a,b),c)
static int xioctl(int fd, int request, void *arg)
{
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}


//3x3 square dilate
void dilate_square(SDL_Surface* image, SDL_Surface* result)
{
    Uint8 r,g,b;

    for (int i=2; i < (result->w - 2); i++)
    {
        for (int j=2; j< (result->h - 2); j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(uull,result->format, &r ,&g, &b);
            if (r == 255)// It's not auto. white it can be gray unless u work
                // On a binary image
            {
                uull = SDL_MapRGB(result->format, 255, 255, 255);
                put_pixel(result, i, j, uull);
                put_pixel(result, i-2, j, uull);
                put_pixel(result, i-1, j, uull);
                put_pixel(result, i, j-2, uull);
                put_pixel(result, i, j-1, uull);
                put_pixel(result, i+2, j, uull);
                put_pixel(result, i+1, j, uull);
                put_pixel(result, i, j+2, uull);
                put_pixel(result, i, j+1, uull);
                put_pixel(result, i+1, j+1, uull);
                put_pixel(result, i-1, j+1, uull);
                put_pixel(result, i+1, j-1, uull);
                put_pixel(result, i-1, j-1, uull);
            }
        }
    }
}

//erosion 3x3 square
void erode_square(SDL_Surface* image, SDL_Surface* result)
{
    Uint8 r,g,b;
    for (int i= 2; i < image->w - 2; i++)
    {
        for (int j= 2; j< image->h - 2; j++)
        {
            Uint32 uull = get_pixel(image, i, j); //from pixel.c
            //to be able to easily manipulate pixels
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if (r == 0) // Same thing, it can be gray
            {
                uull = SDL_MapRGB(result->format, 255, 255, 255);
                put_pixel(result, i, j, uull);
                put_pixel(result, i-2, j, uull);
                put_pixel(result, i-1, j, uull);
                put_pixel(result, i, j-2, uull);
                put_pixel(result, i, j-1, uull);
                put_pixel(result, i+2, j, uull);
                put_pixel(result, i+1, j, uull);
                put_pixel(result, i, j+2, uull);
                put_pixel(result, i, j+1, uull);
                put_pixel(result, i+1, j+1, uull);
                put_pixel(result, i-1, j+1, uull);
                put_pixel(result, i+1, j-1, uull);
                put_pixel(result, i-1, j-1, uull);
            }
        }
    }
    for(int i = 0; i < result->w; i++)
    {
        for(int j =0; j < result->h; j++)
        {
            Uint32 uull = get_pixel(result, i, j);
            SDL_GetRGB(uull,image->format, &r ,&g, &b);
            if(r == 255) // White pixel
            {
                uull = SDL_MapRGB(result->format, 0,0,0);
                put_pixel(result, i,j,uull);
            }
            else
            {
                uull = SDL_MapRGB(result->format, 255,255,255);
                put_pixel(result, i, j, uull);
            }
        }
    }
}


int print_caps(int fd)
{
    struct v4l2_capability caps = {};
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps))
    {
        perror("Querying Capabilities");
        return 1;
    }

    printf( "Driver Caps:\n"
            "  Driver: \"%s\"\n"
            "  Card: \"%s\"\n"
            "  Bus: \"%s\"\n"
            "  Version: %d.%d\n"
            "  Capabilities: %08x\n",
            caps.driver,
            caps.card,
            caps.bus_info,
            (caps.version>>16)&&0xff,
            (caps.version>>24)&&0xff,
            caps.capabilities);


    struct v4l2_cropcap cropcap = {0};
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
    {
        perror("Querying Cropping Capabilities");
        return 1;
    }

    printf( "Camera Cropping:\n"
            "  Bounds: %dx%d+%d+%d\n"
            "  Default: %dx%d+%d+%d\n"
            "  Aspect: %d/%d\n",
            cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, \
                cropcap.bounds.top,
            cropcap.defrect.width, cropcap.defrect.height, \
                cropcap.defrect.left, cropcap.defrect.top,
            cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);

    int support_grbg10 = 0;

    struct v4l2_fmtdesc fmtdesc = {0};
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    char fourcc[5] = {0};
    char c, e;
    printf("  FMT : CE Desc\n--------------------\n");
    while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
        if (fmtdesc.pixelformat == V4L2_PIX_FMT_MJPEG)
            support_grbg10 = 1;
        c = fmtdesc.flags & 1? 'C' : ' ';
        e = fmtdesc.flags & 2? 'E' : ' ';
        printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
        fmtdesc.index++;
    }

    if (!support_grbg10)
    {
        printf("Doesn't support GRBG10.\n");
        return 1;
    }

    //struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 752;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        perror("Setting Pixel Format");
        return 1;
    }

    strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
    printf( "Selected Camera Mode:\n"
            "  Width: %d\n"
            "  Height: %d\n"
            "  PixFmt: %s\n"
            "  Field: %d\n",
            fmt.fmt.pix.width,
            fmt.fmt.pix.height,
            fourcc,
            fmt.fmt.pix.field);
    return 0;
}

int init_mmap(int fd)
{
    struct v4l2_requestbuffers req = {0};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)){
        perror("Requesting Buffer");
        return 1;
    }

    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf)){
        perror("Querying Buffer");
        return 1;
    }

    buffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, \
            buf.m.offset);
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);

    return 0;
}

int capture_image(int fd)
{
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(fd, VIDIOC_QBUF, &buf)){
        perror("Query Buffer");
        return 1;
    }

    if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type)){
        perror("Start Capture");
        return 1;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2;
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if(-1 == r){
        perror("Waiting for Frame");
        return 1;
    }

    if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf)){
        perror("Retrieving Frame");
        return 1;
    }
    return 0;
}

float Max3(float a, float b, float c)
{
    if (a >= b && a >= c)
        return a;
    else if (b >= a && b >= c)
        return b;
    return c;
}



void image_conversion(SDL_Surface* image)
{
    size_t width = image->w;
    size_t height = image->h;
    for(size_t i = 0; i < width; i++)
    {
        for(size_t j = 0; j < height; j++)
        {
            Uint8 r,g,b;
            Uint32 pixel = get_pixel(image, i, j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            float R_p, G_p, B_p;
            R_p = (float)r/(float)(r+g+b);
            G_p = (float)g/(float)(r+g+b);
            B_p = (float)b/(float)(r+g+b);

            float H,S,V;
            V = MAX3(R_p, G_p, B_p);

            if (V != 0)
                S = (V- MIN3(R_p, G_p, B_p))/V;
            else
                S = 0;

            if (V == R_p)
                H = 60*(G_p-B_p)/(V-MIN3(R_p, G_p, B_p));
            else if (V == G_p)
                H = 2 + 60*(B_p - R_p)/(V - MIN3(R_p, G_p, B_p));
            else if (V == B_p)
                H = 4 + 60*(R_p - G_p)/(V - MIN3(R_p, G_p, B_p));

            if (H < 0)
                H = H + 360;

            float Y,Cb,Cr;
            to_ycbcr(r,g,b,&Y,&Cb,&Cr);


            if (R_p/G_p > 1.185 && (0.2 <= S && S <= 0.6) && \
                    ((0 <= H && H <= 25) || (335 <= H && H <= 360)) && \
                    (77 < Cb && Cb < 127) && (133 < Cr && Cr < 173))
                pixel = SDL_MapRGB(image->format, 255 ,255,255);
            else
                pixel = SDL_MapRGB(image->format, 0, 0, 0);

            put_pixel(image, i, j, pixel);

        }
    }

}


void sdlInit() // Init SDL with frame height and width
{
    if(SDL_Init(SDL_INIT_VIDEO))
        perror ("SDL_Init error occured !");

    IMG_Init(IMG_INIT_JPG);



    screen = SDL_SetVideoMode(
            fmt.fmt.pix.width,
            fmt.fmt.pix.height,
            32, SDL_HWSURFACE );



    screen_dilation = SDL_SetVideoMode(
            fmt.fmt.pix.width,
            fmt.fmt.pix.height,
            32, SDL_HWSURFACE );
}





void sdlUpdate() // Update the SDL_Surface with a new frame
{
    // Load from the v4l2 webcam buffer
    buffer_stream = SDL_RWFromMem(buffer, buf.bytesused);

    frame = IMG_Load_RW(buffer_stream, 0);

    // ################### APPLY ##########################
    // Tout ROB, rouge HOUGH

    SDL_Surface* rob_surface = to_rob(frame); // Robert Edge detection call

    //image_conversion(frame); // Black and white in Frame (in place)



    /* IMAGE DILATAION AND EROSION
    SDL_Surface* dilatation_surface = \
            new_rgb_surface(fmt.fmt.pix.width, fmt.fmt.pix.height);
    SDL_Surface* erode_surface = \
            new_rgb_surface(fmt.fmt.pix.width, fmt.fmt.pix.height);

    dilate_square(frame,dilatation_surface); // Dilate frame in result (in place)
    erode_square(dilatation_surface, erode_surface);// Dilate result in result1 (in place)
    */


    SDL_BlitSurface(rob_surface, NULL, screen, &position); // Show result1
    SDL_Flip(screen);
}








void test(SDL_Surface* frames)
{
    SDL_BlitSurface(frames, NULL, screen, &position);
    SDL_Flip(screen);
}


void sdlStop() // Stop SDL and free the surface !
{
    SDL_FreeSurface(frame);
    SDL_RWclose(buffer_stream);
    IMG_Quit();
    SDL_Quit();
}


int main()
{
    int fd;

    fd = open("/dev/video0", O_RDWR);
    if (fd == -1){
        perror("Opening video device");
        return 1;
    }

    if(print_caps(fd))
        return 1;

    sdlInit();

    if(init_mmap(fd))
        return 1;

    for(int i = 0; i != 10000; i++){
        if(capture_image(fd))
            return 1;
        sdlUpdate();
        usleep(3000);
    }

    sdlStop();
    close(fd);
    return 0;
}
