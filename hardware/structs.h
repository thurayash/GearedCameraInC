#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#define turn_onB(n)          {PORTB |= (1 << n);}
#define turn_offB(n)         {PORTB &= ~(1 << n);}
#define set_outputB(n)       {DDRB |= (1 << n);}
#define set_inputB(n)        {DDRB &= ~(1 << n);}

#define set_outputD(n)       {DDRD |= (1 << n);}
#define turn_onD(n)          {PORTD |= (1 << n);}
#define turn_offD(n)         {PORTD &= ~(1 << n);}
#define set_inputD(n)        {DDRD &= ~(1 << n);}

#define set_outputC(n)       {DDRC |= (1 << n);}
#define turn_onC(n)          {PORTC |= (1 << n);}
#define turn_offC(n)         {PORTC &= ~(1 << n);}
#define set_inputC(n)        {DDRC &= ~(1 << n);}

/* MICROSTEPPING : Multiply by 1000
    full => 200 steps => 1.8
    half => 400 steps => 0.9
    1/4  => 800 steps => 0.45
    1/8  => 1600 steps=> 0.225
    1/16 => 3200 steps=> 0.1125
*/

struct Motor{
    char* axis;
    int pin_dir;
    int pin_step;
    int direction; // clockwise or anti_clockwise
    float steps; // Microstepping
    int speed; // ms
};



struct Motor* new_motor(int pin_step, int pin_dir, float steps,\
        int dir, char* name);

void motor_turn(struct Motor* m1, float angle);

void my_delay(int);
#endif
