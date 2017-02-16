//* testing TIMER1 interrupt - main.c *
#include "bios_timer_period.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdint.h>
#include <avr/interrupt.h>




int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_capture_initialize();   

    sei();
 
    while(1)
    {
        while ( !isTimingReady() ) ;
        resetTimingReady();
        
        // some testing of time period between two consecutive 
        // presses of a button connected to Timer1 capture input (PD.4)
        uint16_t inseconds = getTime_Period() / 1000000L;
        if (inseconds>255) inseconds=255;
        led8_set(inseconds);
        // this will display the time in seconds between subsequent releases of the button
    }

    return(0);
}


