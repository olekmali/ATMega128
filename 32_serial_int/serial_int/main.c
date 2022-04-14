//* testing USART1 with receive data interrupt - main.c *
#include "bios_uart1_int.h"
#include "bios_led8.h"
#include <stdlib.h>
#include <avr/interrupt.h>


void MyCharReceivedFN (char c)
{
    // This function is called from within an interrupt
    // It must execute quickly - no loops or delays

    // indicate that an interrupt is called by displaying the 7-bit char number
    // the 8th bit is preserved for toggling in main
    led8_set( ( led8_get()&0b10000000 )  |  ( c & 0b01111111 ) );

    if ( uart1_ready_TX() ) uart1_putc(c); // echo only if that would not stall
}


int main(void)
{
    uart1_initialize(uart_bps_9600, MyCharReceivedFN);

    led8_init();
    led8_set(0);

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event

    while (1)
    {
        cli();
        // This reads and writes the same resource that is used inside the interrupt
        // The interrupt must not suspend this sequence of operations and modify the resource mean while
        // or data integrity will be lost in such rare but possible case
        led8_set( led8_get()^0b10000000 );  // indicate that the main loop is "still alive"
        sei();

        delay(500);
    }
    return(0);
}


