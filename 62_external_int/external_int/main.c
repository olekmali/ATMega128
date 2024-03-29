//* testing TIMER1 interrupt - main.c *
#include "bios_external_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdlib.h>
#include <avr/interrupt.h>


void MyFN45 (void)
{
    // This demo function shows how to associate an action with a particular interrupt
    led8_set(0); // reset LEDs, this will also reset the count in main loop
}

void MyFN67 (void)
{
    // This demo function shows how to associate action with several interrupts
    //      and check the source of the interrupt assuming pin_low activation.
    // In general it might be easier to have independent functions set for each pin.

    uint8_t inputs = key4_get();
    uint8_t status = led8_get();
    // uncomment only one of the two lines for the demo
    status = status ^ (inputs<<4);      // this will show that buttons bounce and trigger multiple times
    // status = status | (inputs<<4);   // this will show that the interrupt was triggered at least once
    led8_set( status ); // set LEDs based on input
}



int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    External_Int4_initialize(EXT_INT_MODE_pin_falle, MyFN45);   // test each of the modes - this triggered once
    External_Int5_initialize(EXT_INT_MODE_pin_low,   MyFN45);   // test each of the modes - this triggered over and over again while button pressed

    External_Int6_initialize(EXT_INT_MODE_pin_falle, MyFN67);
    External_Int7_initialize(EXT_INT_MODE_pin_falle, MyFN67);
    sei(); // enable global interrupts, all circuitry must be initialized prior to this event

    uint8_t keep_busy = 0;
    while(1)
    {
        keep_busy++;
        if (keep_busy>=16)
            keep_busy = 0;

        cli();
        led8_set( (led8_get() & 0xF0 ) | keep_busy );
        sei();
        delay(100);

    }

    return(0);
}
