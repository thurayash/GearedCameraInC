#include "tone.h"

// global variable to determine the beep duration
static volatile uint32_t duration = 0x00;


ISR(TIMER0_COMPA_vect) {
    if (!duration) {
        // mask the interrupt
        TIMSK0 &= ~_BV(OCIE0A);

        // disable clock
        TCCR0B = 0x00;

        // set the port state to low, when generation is finished
        turn_offD(PD6);
    }
    else {
        duration--;
    }
}


static void timer_freq_prescale(uint32_t a_freq, uint8_t *a_ocr, uint8_t *a_prescaler) {
    // prescaler table for timer 0
    uint8_t prescalers[] = { 0x00, 0x03, 0x06, 0x08, 0x0a, 0x00 };

    uint16_t ocr = 0x00;
    uint8_t prescaler = 0x00;

    do {
        ocr = (uint16_t) (F_CPU / ((a_freq << 1) * (0x01 << prescalers[prescaler])));
        ++prescaler;
    } while ((ocr > 255) && (prescalers[prescaler]));

    --ocr;
    if (ocr > 255) ocr = 255;

    *a_ocr = ocr & 0xff;
    *a_prescaler = prescaler;
}


void beep(uint16_t a_freq, uint16_t a_duration) {
    uint8_t prescaler = 0x00,
            ocr = 0x00;

    sei();
    power_timer0_enable();

    // configure the COMP0A pin as output
    set_outputD(PD6);

    // calculate the frequency
    timer_freq_prescale(a_freq, &ocr, &prescaler);

    // COM0A1/COM0A0 = 0x01 - toggle OC0A on Compare Match, mode = CTC
    TCCR0A = 0x42;
    OCR0A = ocr;
    TCNT0 = 0x00;

    // configure the prescaler, now the timer is running and the tone is being generated
    TCCR0B = prescaler & 0x07;

    // calculate the amount of cycles
    duration = (uint32_t)((a_freq * a_duration)/500);

    // unmask the interrupt
    TIMSK0 |= _BV(OCIE0A);

    // block until the tone fades out
    while(duration);
}
