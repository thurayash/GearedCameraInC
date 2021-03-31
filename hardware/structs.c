#include "structs.h"



void my_delay(int n) {
 while(n--) {
  _delay_ms(1);
 }
}

struct Motor* new_motor(int pin_step, int pin_dir, float steps,\
        int dir, char* name)
{
    struct Motor* m1 = malloc(sizeof(struct Motor));

    set_outputD(pin_dir);
    set_outputD(pin_step);

    m1->direction = dir;
    m1->axis = name;
    m1->pin_dir = pin_dir;
    m1->pin_step = pin_step;

    m1->speed = 10;
    m1->steps = steps;
    return m1;
}

void motor_turn(struct Motor* m1, float angle)
{

    // Step determination
    int nbr_steps = (int)(angle/m1->steps);
    char number_str[10];

    // Setting the direction
    if (m1->direction)
    {
        turn_onD(m1->pin_dir);
    }
    else
    {
        turn_offD(m1->pin_dir);
    }


    // Launching Rotation
    for(; nbr_steps > 0; nbr_steps--)
    {
        turn_onD(m1->pin_step);
        my_delay(m1->speed);
        turn_offD(m1->pin_step);
        my_delay(m1->speed);
    }
    return (void)NULL;
}


void free_motor(struct Motor* m1)
{
    free(m1);
}
