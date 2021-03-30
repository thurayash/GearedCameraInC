#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#define turn_onB(n)          {PORTB |= (1 << n);}
#define turn_offB(n)         {PORTB &= ~(1 << n);}
#define set_outputB(n)       {DDRB |= (1 << n);}
#define set_inputB(n)        {DDRB &= ~(1 << n);}

#define set_outputD(n)      {DDRD |= (1 << n);}
#define turn_onD(n)         {PORTD |= (1 << n);}
#define turn_offD(n)        {PORTD &= ~(1 << n);}
#define set_inputD(n)        {DDRD &= ~(1 << n);}

#define set_outputC(n)      {DDRC |= (1 << n);}
#define turn_onC(n)         {PORTC |= (1 << n);}
#define turn_offC(n)        {PORTC &= ~(1 << n);}
#define set_inputC(n)        {DDRC &= ~(1 << n);}

enum Direction{
    CLOCKWISE = 1,
    ANTI_CLOCWISE = 0
};

enum Mode {
    FULL          =   2,
    HALF          =   4,
    ONE_QUARTER   =   8,
    ONE_EIGHTH     =  16,
    ONE_SIXTEENTH  =  32
};

/* MICROSTEPPING :
    full => 200 steps
    half => 400 steps
    1/4  => 800 steps
    1/8  => 1600 steps
    1/16 => 3200 steps
*/




enum Microstepping {
    // Angle by step needs to be /10000
    FULL_step = 18000, // 200 steps
    HALF_step = 9000, // 400 steps
    ONE_QUARTER_step = 4500, // 800 steps
    ONE_EIGHTH_step = 2250, // 1600 steps
    ONE_SIXTEENTH_step = 1125 // 3200 steps
};

typedef struct {
    char* axis;
    int pin_dir;
    int pin_step;
    enum Direction direction; // clockwise or anti_clockwise
    enum Mode mode; // FULL, HALF, ONE_QUARTER, ONE_EIGHTH, ONE_SIXTEENTH
    enum Microstepping steps; // Microstepping
    int speed; // ms
} Motor;



void new_motor(Motor*, int pin_step, int pin_dir, enum Mode mode, \
        enum Direction dir, char* name);


void motor_turn(Motor* m1, float angle);
#endif
