#include "blink.h"

void blink_function()
{
    //PORTB ^= _BV(PB5);
    turn_onB(PB5);
    _delay_ms(1000);
    turn_offB(PB5);
    _delay_ms(1000);
    return ;
}
