#ifndef _THREAD_H_
#define _THREAD_H_

#include "dilation_erosion.h"
#include "circle_detection.h"
#include "image_conversion.h"
#include "init.h"

void* thread_skin_func(void* arg __attribute__((unused)));

void* thread_circle_func(void* arg __attribute__((unused)));

void* thread_send_angle(void* arg __attribute__((unused)));
#endif
