//* testing TIMER1 interrupt with software generated PWM - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "bios_led8.h"
#include "bios_key4.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define PWM_FREQUENCY       1000L
#define PWM_RESOLUTION      5
                        //  ^^^ PWM resolution which is ( 100% / PWM_RESOLUTION% )
                        //  ^^^ In this case it is 20%, the levels are 0% 20% 40% 60% 80% 100%
#define INT_FREQUENCY       (PWM_FREQUENCY * PWM_RESOLUTION)
#define MAIN_LOOP_FREQUENCY 100


//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t     semaphore = 0;  // volatile keyword is very important here!

static volatile uint8_t     pwm0 = 0;  // volatile keyword is important here!
static volatile uint8_t     pwm1 = 0;  // volatile keyword is important here!
// Note: if a shared variable is larger than 8bit then its update is not so called atomic
//      and interrupts have to be suspended while a variable is modified in main function

//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // Remember: we are inside the interrupt routine
    //              - no excessive loops
    //              - no time consuming tasks
    //              - no interrupt enabling

    static uint8_t pwm_rate0 = 0;   // PWM rate only for the current time period
    static uint8_t pwm_rate1 = 0;   // PWM rate only for the current time period
    static uint8_t pwm_counter = 0; // note: static == hidden global variable
    //     ^^^^^^^^ make sure that ( SAMPLING__FRQ / MAIN_LOOP_FREQUENCY )
    // fits the variable range! uint8_t - 255, uint16_t - 65535
    if (0<pwm_counter) {
        pwm_counter--;
    } else {
        pwm_counter = (PWM_RESOLUTION-1);
        // we do not want PWM rate to change in the middle of a PWM cycle
        pwm_rate0 = pwm0;
        pwm_rate1 = pwm1;
    }

    // PWM action section
    uint8_t curLEDs = led8_get();
    if ( pwm_counter<pwm_rate0 ) {
        curLEDs = curLEDs | 0b00000001;
        } else {
        curLEDs = curLEDs & 0b11111110;
    }
    if ( pwm_counter<pwm_rate1 ) {
        curLEDs = curLEDs | 0b00000010;
        } else {
        curLEDs = curLEDs & 0b11111101;
    }
    led8_set(curLEDs);


    static uint8_t semaphore_counter = 0; // note: static == hidden global variable
    //     ^^^^^^^^ make sure that ( SAMPLING__FRQ / MAIN_LOOP_FREQUENCY )
    // fits the variable range! uint8_t - 255, uint16_t - 65535
    if (0<semaphore_counter) {
        semaphore_counter--;
        } else {
        semaphore_counter = ( INT_FREQUENCY / MAIN_LOOP_FREQUENCY );
        semaphore = 1;
    }
}


int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1 );

    // Set the PWM rates for the PWM channels
    // Note: PWMs needs to be in range 0...PWM_RESOLUTION, not in %
    pwm0 = 0;
    pwm1 = 100;

    sei();

    uint8_t but_prev = 0;
    while(1) {
        // wait for the signal to proceed from the interrupt
        while ( semaphore==0 )
            ;
        // and immediately reset that signal to wait state
        semaphore = 0;

        uint8_t but_cur = key4_get();
        uint8_t but_chg = ( but_cur^but_prev ) & but_cur;
        but_prev = but_cur; // important! update what is now current will be past next time

        // cycle through five PWM levels with one button
        if ( (but_chg & B_K4) !=0 ) { // if ( (but_chg & 0b00000001) !=0 )
            if (pwm0<PWM_RESOLUTION) {
                pwm0++;
            } else {
                pwm0 = 0;
            }
        } // else nothing. Technically several buttons may be depressed during the same time interval

        if ( (but_chg & B_K6) !=0 ) { // if ( (but_chg & 0b00000100) !=0 )
            if (pwm1<PWM_RESOLUTION) {
                pwm1++;
            } // else nothing;
        } // else nothing. Technically several buttons may be depressed during the same time interval

        if ( (but_chg & B_K7) !=0 ) { // if ( (but_chg & 0b00001000) !=0 )
            if (pwm1>0) {
                pwm1--;
            } // else nothing;
        } // else nothing. Technically several buttons may be depressed during the same time interval

    }

    return(0);
}
