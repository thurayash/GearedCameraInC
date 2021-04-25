#include "main.h"
#include <string.h>
#include "json_parser.h"

int main()
{
    USART_Init();

    set_outputB(PB5);
    set_outputB(PB4);

    struct JSON* parser = new_json();

    struct Motor* m1 = new_motor(PD4, PD3, 1.8f , 1, "X");

    while (1)
    {
        char text[128];
        read_string(text);

        json_fromstr(parser, text);

        if (parser->name == 'X')
        {
            turn_onB(PB4);
            m1->direction = parser->direction;
            m1->speed = parser->speed;
            motor_turn(m1, (float)parser->radius);
            turn_onB(PB5);
            turn_offB(PB4);
        }
    }
    free(parser);
    free(new_motor);
    return 0;
}
