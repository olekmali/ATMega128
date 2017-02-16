//* testing TIMER1 interrupt - main.c *
#include "bios_external_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdlib.h>
#include <avr/interrupt.h>


void MyFN4 (void)
{
    // This demo function shows how to associate an action with a particular interrupt
    led8_set(0); // reset LEDs
}

void MyFN567 (void)
{
    // This demo function shows how to associate action with several interrupts
    //      and check the source of the interrupt assuming pin_low activation.
    // In general it might be easier to have four independent functions set. 
    static uint8_t old_state = 0;
    uint8_t state = key4_get();
    uint8_t change = state ^ old_state;
    state = old_state;

    led8_set( change ); // set LEDs based on input 
}



int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

//  External_Int0_initialize(EXT_INT_MODE_pin_low, MyFN);
//  External_Int1_initialize(EXT_INT_MODE_pin_low, MyFN);
    External_Int4_initialize(EXT_INT_MODE_pin_low, MyFN4);
    External_Int5_initialize(EXT_INT_MODE_pin_low, MyFN567);
    External_Int6_initialize(EXT_INT_MODE_pin_low, MyFN567);
    External_Int7_initialize(EXT_INT_MODE_pin_low, MyFN567);
    sei(); // enable global interrupts, all circuitry must be initialized prior to this event

    while(1)
    {
        
    }

    return(0);
}
