#include "main.h"
#include "tools.h"


int main(int argc, char** argv)
{
    if(argc < 2)
        errx(EXIT_FAILURE, "Needs a mode !\n\
                \n0: Basic webcam V4L2\
                \n1: Skin detection noisy\
                \n2: Skin detection dilated and eroded\
                \n3: Robert edges detection grayscale\
                \n4: Real roberts edge detection\
                \n5: Hough Transform CircleDetection");
    int fd;

    int mode = atoi(argv[1]);

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
        sdlUpdate(mode);
        usleep(3000);
    }

    sdlStop();
    close(fd);
    return 0;
}
