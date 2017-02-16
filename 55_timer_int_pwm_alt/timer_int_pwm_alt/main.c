//* testing TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "soft_pwm_ALT.h"
#include "bios_led8.h"
#include "bios_key4.h"

#define INT_FREQUENCY       (200000L)
#define PWM_FREQUENCY       (1000L)
#define KEY_SCAN_FREQUENCY  (50)

#define oneMilliSecond  (PWM_FREQUENCY * PWM_RESOLUTION / 1000)
#define ACTIONTIME      (100*oneMilliSecond)


void ButtonMenuController(void)
{
    /*
        This is the place to implement functionality of key-based menu like one in the Timer1 
        interrupt example that demonstrates button polling with short, long and double-click 
        button detection. Such function can access and alter PWM setting for each channel.
        One button can be used to walk among the channels round robin style, while two other
        buttons can be used to increase or decrease the PWM parameter for the current channel.
    */
}


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // Remember: we are inside the interrupt routine
    //              - no excessive loops
    //              - no time consuming tasks
    //              - no interrupt enabling
    PWM_generator_interrupt();

    static uint16_t counter = 0;            // make sure that ( SAMPLING__FRQ / 100 ) fits the variable range!
    if (0<counter) counter--;
    else
    {
        counter = ( INT_FREQUENCY / KEY_SCAN_FREQUENCY );

        ButtonMenuController();             // this must be called with the desired button scanning rate
    }
}


int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FREQUENCY , MyTimerFN );   

    PWM_generator_setFreq(INT_FREQUENCY, PWM_FREQUENCY);
    
    sei();

    PWM_generator_setCoef (0, 1);
    PWM_generator_setCoef (1, 2);
    PWM_generator_setCoef (2, 10);
    PWM_generator_setCoef (3, 20);
    PWM_generator_setCoef (4, 50);
    PWM_generator_setCoef (5, 100);
    
    while(1) {
    }

    return(0);
}
