#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include "transmitter.h"
#include <string.h>


#define turn_onB(n)          {PORTB |= (1 << n);}
#define turn_offB(n)         {PORTB &= ~(1 << n);}
#define set_outputB(n)       {DDRB |= (1 << n);}
#define set_inputB(n)        {DDRB &= ~(1 << n);}
#define is_onB(n, val)       {val = !(PINB & (1 << n));}

#define set_outputD(n)       {DDRD |= (1 << n);}
#define turn_onD(n)          {PORTD |= (1 << n);}
#define turn_offD(n)         {PORTD &= ~(1 << n);}
#define set_inputD(n)        {DDRD &= ~(1 << n);}
#define is_onD(n, val)       {val = !(PIND & (1 << n));}


#define set_outputC(n)       {DDRC |= (1 << n);}
#define turn_onC(n)          {PORTC |= (1 << n);}
#define turn_offC(n)         {PORTC &= ~(1 << n);}
#define set_inputC(n)        {DDRC &= ~(1 << n);}

/* MICROSTEPPING : Multiply by 1000
    full => 200 steps => 1.8 deg/step
    half => 400 steps => 0.9 deg/step
    1/4  => 800 steps => 0.45  deg/step
    1/8  => 1600 steps=> 0.225  deg/step
    1/16 => 3200 steps=> 0.1125 deg/step
*/

struct Motor{
    char axis;
    unsigned int pin_dir : 4;
    unsigned int pin_step : 4;
    unsigned int direction : 1; // clockwise or anti_clockwise
    float steps; // Microstepping
    unsigned int speed : 5; // ms
    unsigned int pin_stop : 5;
    unsigned int prev : 1;
};



int init_motor(struct Motor* m1, struct Motor* m2);


struct Motor* new_motor(int pin_step, int pin_dir, float steps,\
        int dir, char name, int pin_stop);

int  motor_turn(struct Motor* m1, float angle);

void motor_turn_double(struct Motor* m1, struct Motor* m2,
                    float angle1, float angle2);

void my_delay(int);

#endif
