#ifndef VIDTEST_H_
#define VIDTEST_H_

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "tools.h"
#include "rgb_to_hsi.h"
#include "image_conversion.h"

#include "dilation_erosion.h"

#include "roberts_edge.h"

#include "circle_detection.h"
#define MAX3(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? \
            (n) : (p)))

#define MIN(a,b) ((a) < (b)  ? (a) : (b) )
#define MIN3(a,b,c) MIN(MIN(a,b),c)


int print_caps(int fd);

int init_mmap(int fd);

int capture_image(int fd);

void sdlInit();

void sdlUpdate(int);

void sdlStop();

int vidtest_camCheck(char* cameraPath);

#endif