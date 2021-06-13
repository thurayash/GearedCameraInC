#include "structs.h"


void my_delay(int n) {
    while(n--) {
        _delay_ms(1);
    }
}


int init_motor(struct Motor* m1, struct Motor* m2){
    int result = 0;
    if (motor_turn(m1, 2950.f))
    {
        motor_turn(m1, 1475.f);
        result += 1;
    }
    if (motor_turn(m2, 3360.f))
        result += 2;
    if (result == 3)
        result += 4;
    return result;
}



struct Motor* new_motor(int pin_step, int pin_dir, float steps,\
        int dir, char name, int pin_stop)
{
    struct Motor* m1 = malloc(sizeof(struct Motor));

    set_outputC(pin_dir);
    set_outputC(pin_step);

    m1->direction = dir;
    m1->axis = name;
    m1->pin_dir = pin_dir;
    m1->pin_step = pin_step;
    m1->prev =  0 ;

    m1->speed = 1;
    m1->steps = steps;

    if (name == 'X'){
        m1->pin_stop = pin_stop;
        set_inputB(pin_stop);
    }
    else{
        m1->pin_stop = pin_stop;
        set_inputD(pin_stop); // Y
    }
    return m1;
}

int motor_turn(struct Motor* m1, float angle)
{
    // Step determination
    int nbr_steps = (int)(angle/m1->steps);

    // Setting the direction
    if (m1->direction){
        turn_onC(m1->pin_dir);
    }
    else{
        turn_offC(m1->pin_dir);
    }

    // Launching Rotation
    int val = 0;
    for(; nbr_steps > 0; nbr_steps--)
    {
        if (m1->axis == 'X' && !m1->prev){
            is_onB(m1->pin_stop, val);
            if (val){
                m1->prev = 1;
                m1->direction = !m1->direction;
                return motor_turn(m1, 30.f);
            }
        }else if (!m1->prev){
            is_onD(m1->pin_stop, val);
            if (val){
                m1->prev = 1;
                m1->direction = !m1->direction;
                return motor_turn(m1, 60.f);
            }
        }
        turn_onC(m1->pin_step);
        my_delay(m1->speed);
        turn_offC(m1->pin_step);
        my_delay(m1->speed);

    }
    if (m1->prev){
        m1->prev = 0;
        return 1;
    }
    return 0;
}


void motor_turn_double(struct Motor* m1, struct Motor* m2,
        float angle1, float angle2)
{
    int nbr_steps1 = (int)(angle1/m1->steps);
    int nbr_steps2 = (int)(angle2/m2->steps);

    // Setting the direction for both m1 and m2
    if (m1->direction){
        turn_onC(m1->pin_dir);
    }else{
        turn_offC(m1->pin_dir);
    }

    if(m2->direction){
        turn_onC(m2->pin_dir);
    }else{
        turn_offC(m2->pin_dir);
    }

    int val1 = 0;
    int val2 = 0;
    // Launching Rotation
    while(nbr_steps1 > 0 || nbr_steps2 > 0)
    {
        if(nbr_steps1 > 0)
        {
            if (m1->axis == 'X' && !m1->prev){
                is_onB(m1->pin_stop, val1);
                if(val1){
                    m1->prev = 1;
                    nbr_steps1 = 60.f;
                    m1->direction = !m1->direction;
                    if (m1->direction){
                        turn_onC(m1->pin_dir);
                    }else{
                        turn_offC(m1->pin_dir);
                    }
                }
            }else if (!m1->prev){
                is_onD(m1->pin_stop, val1);
                if(val1){ // Meme si ca a l'air debile comme ca...
                    m1->prev = 1;
                    nbr_steps1 = 60.f;
                    m1->direction = !m1->direction;
                    if(m1->direction){
                        turn_onC(m1->pin_dir);
                    }else{
                        turn_offC(m1->pin_dir);
                    }
                }
            }
            turn_onC(m1->pin_step);
            my_delay(m1->speed);
            turn_offC(m1->pin_step);
            my_delay(m1->speed);
            nbr_steps1--;
        }

        if(nbr_steps2 > 0)
        {
            if (m2->axis == 'X' && !m2->prev){
                is_onB(m2->pin_stop, val2);
                if(val2){
                    m2->prev = 1;
                    nbr_steps2 = 160.f;
                    m2->direction = !m2->direction;
                    if (m2->direction){
                        turn_onC(m2->pin_dir);
                    }else{
                        turn_offC(m2->pin_dir);
                    }
                }
            }else if (!m2->prev){
                is_onD(m2->pin_stop, val2);
                if(val2){ // Meme si ca a l'air debile comme ca...
                    m2->prev = 1;
                    nbr_steps2 = 160.f;
                    m2->direction = !m2->direction;
                    if(m2->direction){
                        turn_onC(m2->pin_dir);
                    }else{
                        turn_offC(m2->pin_dir);
                    }
                }
            }

            turn_onC(m2->pin_step);
            my_delay(m2->speed);
            turn_offC(m2->pin_step);
            my_delay(m2->speed);
            nbr_steps2--;
        }
    }
    m1->prev = 0;
    m2->prev = 0;
    return (void)NULL;
}

void free_motor(struct Motor* m1)
{
    free(m1);
}
