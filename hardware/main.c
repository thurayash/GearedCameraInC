#include "main.h"
#include <string.h>
#include <time.h>


void motor_turn_b(int pin_step, int pin_dir, float angle, \
        enum Direction dir ,enum Microstepping steps, int speed)
{
    // Step determination
    int nbr_steps = (int)(angle/((float)steps/10000.f));
    char number_str[10];

    if (dir)
    {
        turn_onD(pin_dir);
    }
    else
    {
        turn_offD(pin_dir);
    }

    sprintf(number_str, "%i", nbr_steps);
    send_string(number_str);

    // Launching Rotation
    for(; nbr_steps > 0; nbr_steps--)
    {
        turn_onD(PD4);
        _delay_ms(5);
        turn_offD(PD4);
        _delay_ms(5);
    }
    return (void)NULL;
}


int main()
{
    USART_Init();

    set_outputB(PB5);
    set_outputD(PD4);
    set_outputD(PD3);

    char LocalData;
    char number_str[10];

    while (1)
    {
        LocalData = USART_ReceivePolling();

        if (LocalData == 'a')
        {
            motor_turn_b(PD4, PD3, 90, CLOCKWISE, FULL_step, 5);
            send_string("A received !");
            turn_onB(PB5);
        }
        else if (LocalData == '\n')
        {
            send_string("NewLine received !");
        }
        else
        {
            motor_turn_b(PD4, PD3, 360, ANTI_CLOCWISE, FULL_step, 10);
            send_string("Turning off !");
            turn_offB(PB5);
        }
        _delay_ms(1000);

    }
    return 0;
}
