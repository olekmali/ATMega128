//* bios_led8.c *
#include "bios_led8.h"
#include <avr/io.h>

//#define F_CPU (16000000UL)
//#include <util/delay.h>

void led8_init (void)
{
    // set all pins on PORTA for output
    DDRA  = 0xFF;   // 0b11111111
}

void led8_shutdown (void)
{
    // to be safe and low power set the upper nibble back to input and shut off pull up resistors
    DDRA = 0;
    PINA = 0;
}

uint8_t led8_get (void)
{
    // no need for delay after writing to PORTA 
    // as it takes time to call the function
    return(PINA);
}

void led8_set ( uint8_t number )
{
    PORTA=number;
}


void delay (unsigned short int milliseconds)
{
    while (milliseconds>0)
    {
        volatile uint16_t counter;
        for (counter=0; counter<842; counter++) ;
        milliseconds--;
    }
}
