//* bios.c *
#include "bios.h"
#include <avr/io.h>

//#define F_CPU (16000000UL)
//#include <util/delay.h>

void init_digital_io()
{
    // set all pins on PORTA for output
    DDRA  = 0xFF;   // 0b11111111

    // PORTC will be partially for input, and partially for output
    // but for now let'd keep it all input and with pull-up resistors enabled
    DDRC  = 0x00;   // 0b11110000
    PORTC = 0xFF;   // 0b00001111
}


void display_byte (uint8_t number)
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

