//* Timer2 hardware PWM - implementation file *
#include "bios_pwmdac.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer2_PWM_initialize ()
{
    TCNT2 = 0;
    OCR2  = 0;  // PWM at 0%
    TCCR2 = (1<<COM21) |  (1<<WGM21 | 1<<WGM20) | (1<<CS20);
            // Port pin Hi on bottom, Low on compare when fast PWM | Fast PWM mode | Prescale of 1 (fastest)
    // TIMSK = TIMSK & ~(1<<OCIE2|1<<TOIE2);    // interrupts disabled (and were disabled to start with)
    DDRB = DDRB | 1<<7; // set the Timer2 PWM pin direction to output - PORTB7
}

void Timer2_PWM_shutdown ()
{
    DDRB = DDRB & ~(1<<7);
    OCR2 =  0;      // PWM at 0%
    TCCR2 = 0;      // shut down
    TCNT2 = 0;
    TIMSK = TIMSK & ~(1<<OCIE2|1<<TOIE2);
    TIFR  = TIFR  & ~(1<<OCF2|1<<TOV2);
}


void Timer2_PWM_setDuty (uint8_t duty)
{
    // OCR2 is hardware double-buffered in PWM generator mode
    OCR2 = duty;
}

