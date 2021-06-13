#define __DELAY_BACKWARD_COMPATIBLE__

#include "main.h"
#include <string.h>
#include "json_parser.h"


int main()
{
    beep(NOTE_C5, 5000);
    my_delay(100);
    beep(NOTE_G6, 1000);
    my_delay(100);
    beep(NOTE_G5, 1000);
    my_delay(70);

    beep(NOTE_C6, 1000);
    USART_Init();

    struct Motor* m1 = new_motor(PC3, PC2, 0.45f , 0, 'X', PINB1); // Having some problemes here in the 0.45f thing
    struct Motor* m2 = new_motor(PC5, PC4, 0.45f , 0, 'Y', PIND7); // Same problemes here, need to be fixed

    char text[256];


    read_string(text);

    if (!strcmp(text, "OKEY!")){
        char response[20];
        int result = init_motor(m1, m2);
        sprintf(response, "{'S':%i}+", result);
        send_string(response);
        if (result < 7){
            beep(NOTE_G3, 10000);
            my_delay(100);
            beep(NOTE_G3, 10000);

            my_delay(5000);
            return main();
        }
    }

    set_outputB(PB5);
    set_outputB(PB4);

    turn_onB(PB5);
    turn_offB(PB4);

    beep(NOTE_C5, 1000);
    beep(NOTE_D5, 2000);
    beep(NOTE_G5, 3000);

    my_delay(1000);

    beep(NOTE_C6, 10000);
    //turn_onB(PB4);

    turn_offB(PB5);
    my_delay(500);

    // G6
    beep(NOTE_G6, 2000);
    my_delay(20);
    beep(NOTE_G6, 2000);

    my_delay(2000);

    struct JSON* parser1 = new_json();
    parser1->name = 'X';
    struct JSON* parser2 = new_json();
    parser2->name = 'Y';

    //motor_turn_double(m1,m2,13200.f, 13200.f);
    _delay_ms(1000);

    while(1)
    {
        // Waiting for a loop back
        turn_onB(PB4);
        USART_LoopBack();
        turn_offB(PB4);

        char text[256];

        read_string(text);
        send_string(text);
        json_fromstr(parser1, parser2, text);

        m1->direction = parser1->direction;
        m2->direction = parser2->direction;

        turn_onB(PB5);
        motor_turn_double(m1, m2, (float)parser1->radius ,(float)parser2->radius);
        turn_offB(PB5);


        parser1->radius = parser1->radius = 0;
    }
}

