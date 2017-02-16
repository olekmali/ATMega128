//* testing TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_external_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define TIMER1_FRQ  (10)
#define TIMEOUT_MS  (10000)
#define COUNT_AWAKE ( 1L* TIMEOUT_MS * TIMER1_FRQ / 1000 )

volatile uint16_t countdown_to_sleep = COUNT_AWAKE;

void MySleepyTimerFN (void)
{
    static uint8_t count = 0;
    count++;
    if (64<=count)
        count = 0;

    // sleep count down
    if (0<countdown_to_sleep) 
        countdown_to_sleep--;

    // to show some activity
    led8_set( ( led8_get() & 0b11000000 ) | count );
}


void MyExternalIntFN (void)
{
    // reset countdown_to_sleep each time there is a button activity
    countdown_to_sleep = COUNT_AWAKE;
    sleep_disable();
}




int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize(TIMER1_FRQ, MySleepyTimerFN, timer_prescale_256);

    External_Int4_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);
    External_Int5_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);
    External_Int6_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);
    External_Int7_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Note: this will not awake on Timer1 event

    sei();  // enable global interrupts, all circuitry must be initialized prior to this event

    while(1)
    {
        // enter your functionality here 
        // and remember to reset countdown_to_sleep each time there is a user activity of some kind


        uint8_t sreg_save = SREG;   // START make the if statement and sleep_enable one atomic operation
        cli();                      // clear all interrupts
        if (0==countdown_to_sleep)
        {
            sleep_enable();     // or   MCUCR  |= (1 << SE);
            SREG = sreg_save;       // STOP1 enable prior interrupts - make the if statement and sleep_enable one atomic operation
            sleep_cpu();        // or   __asm volatile ("sleep");
           sleep_disable();    // or   MCUCR &= ~(1<<SE);
        }
        SREG = sreg_save;           // STOP2 enable prior interrupts - make the if statement and sleep_enable one atomic operation
    }

    return(0);
}
