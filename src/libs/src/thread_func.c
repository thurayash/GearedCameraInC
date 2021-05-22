#include "../header/thread_func.h"


void* thread_skin_func(void* arg __attribute__((unused))){

    ThreadD* frame_t = (ThreadD*)arg;

    image_conversion(frame_t->frame, frame_t->conv);

    dilate_square(frame_t->conv, frame_t->dilate);

    erode_square(frame_t->dilate, frame_t->eroded);

    return NULL;
}



void* thread_circle_func(void* arg __attribute__((unused))){

    ThreadDC* data = (ThreadDC*)arg;

    circleDectection_dynamicadapt(data->fusion_bin,data->arr, 40);

    return NULL;
}
