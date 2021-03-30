#include "structs.h"

void new_motor(Motor* m1, int pin_step, int pin_dir, enum Mode mode,\
        enum Direction dir, char* name)
{
    enum Microstepping steps;

    set_outputD(pin_dir);
    set_outputD(pin_step);

    m1->axis = name;
    m1->pin_dir = pin_dir;
    m1->pin_step = pin_step;
    m1->mode = mode;

    // Testing
    char number_str[10];

    // End Testing

    switch(mode)
    {
        case FULL:
            steps = FULL_step;
            break;
        case HALF:
            steps = HALF_step;
            break;
        case ONE_QUARTER:
            steps = ONE_QUARTER_step;
            break;
        case ONE_EIGHTH:
            steps = ONE_EIGHTH_step;
            break;
        case ONE_SIXTEENTH:
            steps = ONE_SIXTEENTH_step;
            break;
        default:
            break;
    }

    m1->speed = 200;
    m1->steps = steps;

    sprintf(number_str, "%d", m1->pin_step);
    send_string("INIT : ");
    send_string(number_str);
    send_string("\n");
}
