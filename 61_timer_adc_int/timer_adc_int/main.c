//* testing TIMER1 interrupt triggered ADC interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_adc_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdint.h>
#include <avr/interrupt.h>


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    static uint8_t count =0;
    
    if (count==0)
    {
        count = 10;
        // HERE do things every so many interrupts
        adc_startRound();
    } else {
        count--;
    }
    
    // HERE do things every one interrupt, for example PWM generator
}



int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    adc_initialize();
    Timer1_initialize(1000, MyTimerFN, timer_prescale_1);

    sei();
 
    while(1)
    {
        while ( !isAdcReady() ) ;
        resetAdcReady();
        
        if (key4_get() & B_K7)
            led8_set(adc_getRecent(0));
        else
            led8_set(adc_getRecent(1));
    }

    return(0);
}
