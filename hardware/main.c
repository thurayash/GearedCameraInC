#include "main.h"
#include <string.h>
#include "json_parser.h"


/*
void motor_turn_b(int pin_step, int pin_dir, float angle, \
        enum Direction dir ,enum Microstepping steps, int speed)
{
    // Step determination
    int nbr_steps = (int)(angle/((float)steps/10000.f));
    char number_str[10];

    // Setting the direction
    if (dir)
    {
        turn_onD(pin_dir);
    }
    else
    {
        turn_offD(pin_dir);
    }


    // Launching Rotation
    for(; nbr_steps > 0; nbr_steps--)
    {
        turn_onD(PD4);
        my_delay(speed);
        turn_offD(PD4);
        my_delay(speed);
    }
    return (void)NULL;
}*/


int main()
{
    USART_Init();

    set_outputB(PB5);


    struct JSON* parser = new_json();

    struct Motor* m1 = new_motor(PD4, PD3, 1.8f , 1, "X");

    while (1)
    {
        char text[128];
        read_string(text);

        json_fromstr(parser, text);

        if (parser->name == 'X')
        {
            m1->direction = parser->direction;
            m1->speed = parser->speed;
            motor_turn(m1, (float)parser->radius);
            turn_onB(PB5);
        }

        if (parser->name == 'Y')
        {
            /*motor_turn_b(PD4, PD3, 360, ANTI_CLOCWISE, FULL_step, 10);
            send_string("Turning off !");*/
            turn_offB(PB5);
        }
        if(parser->name == 'P')
            break;
    }
    free_json(parser);
    free_motor(m1);
    return 0;
}
