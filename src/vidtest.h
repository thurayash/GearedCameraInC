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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "tools.h"
uint8_t *buffer;
struct v4l2_format fmt = {0};
struct v4l2_buffer buf = {0};

SDL_Surface* screen ;
SDL_RWops* buffer_stream;
SDL_Surface* frame;
SDL_Rect position = {.x = 0, .y = 0};


int print_caps(int fd);

int init_mmap(int fd);

int capture_image(int fd);

void sdlInit();

void sdlUpdate();

void sdlStop();


#endif
