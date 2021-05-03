/*
 * Made with Hearth - Yassine
 *
 * Make sure to compile the .c file using these flags:
 *
 *          gcc -g vidtest.c -std=gnu99 -lSDL -lSDL_image -O3 -O0 -o wc
 *
*/

#include "vidtest.h"

uint8_t *buffer;
struct v4l2_format fmt = {0};
struct v4l2_buffer buf = {0};

SDL_Surface* screen ;
SDL_Surface* screen_dilation;

SDL_RWops* buffer_stream;
SDL_Surface* frame;

SDL_Rect position = {.x = 0, .y = 0};

static int xioctl(int fd, int request, void *arg)
{
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
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



void binary_operation(SDL_Surface* rob, SDL_Surface* skin);

void sdlUpdate(int mode) // Update the SDL_Surface with a new frame
{
    // Load from the v4l2 webcam buffer
    buffer_stream = SDL_RWFromMem(buffer, buf.bytesused);

    frame = IMG_Load_RW(buffer_stream, 0);

    // ################### APPLY ##########################
    if (mode == 0)
    {
        SDL_BlitSurface(frame, NULL, screen, &position); // Show result1
        SDL_Flip(screen);
        SDL_FreeSurface(frame);
        return;
    }

    if (mode == 1){ // Black and white with noise
        noisy(frame);

        SDL_BlitSurface(frame, NULL, screen, &position); // Show result1
        SDL_Flip(screen);

        SDL_FreeSurface(frame);
    }
    else if(mode == 4){ // Robert edge red
        to_rob(frame);
        SDL_BlitSurface(frame, NULL, screen, &position); // Show result1
        SDL_Flip(screen);

        SDL_FreeSurface(frame);
        return;
    }
    else if(mode == 2){ // Black and white dilated and eroded
        noisy(frame);// Black and white in Frame

        SDL_Surface* dilatation_surface = \
            new_rgb_surface(fmt.fmt.pix.width, fmt.fmt.pix.height);
        SDL_Surface* erode_surface = \
            new_rgb_surface(fmt.fmt.pix.width, fmt.fmt.pix.height);
        dilate_square(frame,dilatation_surface);
        // Dilate frame in result (in place)
        erode_square(dilatation_surface, erode_surface);
        // Dilate result in result1 (in place)

        SDL_BlitSurface(erode_surface, NULL, screen, &position);
        // Show result1
        SDL_Flip(screen);
        SDL_FreeSurface(dilatation_surface);
        SDL_FreeSurface(erode_surface);
        SDL_FreeSurface(frame);
    }
    else if(mode == 3){ // Robert edge grayscale
        SDL_Surface* rob_surface = first_rob(frame);
        SDL_BlitSurface(rob_surface, NULL, screen, &position); // Show result1
        SDL_Flip(screen);
        SDL_FreeSurface(rob_surface);
        //SDL_FreeSurface(frame);
    }
    else if (mode == 5)
    {
        SDL_Surface* skin_detect =  new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);
        image_conversion(frame, skin_detect);

        to_rob(frame);

        SDL_Surface* dilatation_surface = new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        SDL_Surface* erode_surface = new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        dilate_square(skin_detect,dilatation_surface);

        erode_square(dilatation_surface, erode_surface);

        //int resx, resy = 0;

        SDL_Surface* red_dilate =  new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        dilate_square_red(frame, red_dilate);

        binary_operation(red_dilate, erode_surface);

        //circleDectection_staticadapt(erode_surface, &resx, &resy);

        //int resx1;
        //int resy1;
        int arr[10];

        circleDectection_dynamicadapt(erode_surface,arr, 5);


        //draw_rectangle(erode_surface, resx, resy, 50, 0);


        //int arr[6];

        //circleDectection3_staticadapt(erode_surface, arr);

        draw_rectangle(erode_surface, arr[0], arr[1], 75, 255, 0 , 0, 0);
        draw_rectangle(erode_surface, arr[2], arr[3], 75, 255, 127, 127, 0);
        draw_rectangle(erode_surface, arr[4], arr[5], 75, 0, 255 , 0, 0);
        draw_rectangle(erode_surface, arr[6], arr[7], 75, 0, 0 , 255, 0);
        draw_rectangle(erode_surface, arr[8], arr[9], 75, 0, 255 , 255, 0);


        SDL_BlitSurface(erode_surface, NULL, screen, &position);
        SDL_Flip(screen);

        SDL_FreeSurface(dilatation_surface);
        SDL_FreeSurface(erode_surface);
        SDL_FreeSurface(skin_detect);
        SDL_FreeSurface(frame);
        SDL_FreeSurface(red_dilate);

        return;
    }
    else if(mode == 6)
    {
        SDL_Surface* skin_detect =  new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);
        image_conversion(frame, skin_detect);

        to_rob(frame);

        SDL_Surface* dilatation_surface = new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        SDL_Surface* erode_surface = new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        dilate_square(skin_detect,dilatation_surface);

        erode_square(dilatation_surface, erode_surface);

        //int resx, resy = 0;

        SDL_Surface* red_dilate =  new_rgb_surface(fmt.fmt.pix.width,
                fmt.fmt.pix.height);

        dilate_square_red(frame, red_dilate);

        binary_operation(red_dilate, erode_surface);

        //circleDectection_staticadapt(erode_surface, &resx, &resy);

        //int resx1;
        //int resy1;
        int arr[10];

        circleDectection_dynamicadapt(erode_surface,arr, 5);


        //draw_rectangle(erode_surface, resx, resy, 50, 0);


        //int arr[6];

        //circleDectection3_staticadapt(erode_surface, arr);

        draw_rectangle(erode_surface, arr[0], arr[1], 75, 255, 0 , 0, 1);
        draw_rectangle(erode_surface, arr[2], arr[3], 75, 255, 127, 127, 1);
        draw_rectangle(erode_surface, arr[4], arr[5], 75, 0, 255 , 0, 1);
        draw_rectangle(erode_surface, arr[6], arr[7], 75, 0, 0 , 255, 1);
        draw_rectangle(erode_surface, arr[8], arr[9], 75, 0, 255 , 255, 1);


        SDL_BlitSurface(erode_surface, NULL, screen, &position);
        SDL_Flip(screen);

        SDL_FreeSurface(dilatation_surface);
        SDL_FreeSurface(erode_surface);
        SDL_FreeSurface(skin_detect);
        SDL_FreeSurface(frame);
        SDL_FreeSurface(red_dilate);

        return;

    }
}


void binary_operation(SDL_Surface* rob, SDL_Surface* skin)
{
    Uint8 r,g,b, rbw;
    for(int i = 0; i < rob->w;i++)
    {
        for(int j = 0; j < rob->h; j++)
        {

            SDL_GetRGB(get_pixel(rob,i,j), rob->format, &r, &g, &b);
            SDL_GetRGB(get_pixel(skin,i,j), skin->format, &rbw, &rbw, &rbw);
            if (r == 255 && rbw == 255)
                put_pixel(skin, i, j, SDL_MapRGB(skin->format, 255,0,0));
        }
    }
}


void debugging(SDL_Surface* frames)
{
    SDL_BlitSurface(frames, NULL, screen, &position);
    SDL_Flip(screen);
}


void sdlStop() // Stop SDL and free the surface !
{
    //SDL_FreeSurface(frame);
    SDL_RWclose(buffer_stream);
    IMG_Quit();
    SDL_Quit();
}



