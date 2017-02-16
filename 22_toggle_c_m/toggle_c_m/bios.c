//* blink and toggle in c - bios.c *
#include "bios.h"

// let us keep the hardware information as enclosed as possible
#include <avr/io.h>

void init_digital_io()
{
    // set all pins on PORTA for output
    DDRA  = 0xFF;

    // set all pins on PORTE for input
    DDRE  = 0;
    // and activate pull up resistors
    PORTE = 0xFF;    
}
    

uint8_t get_buttons()
{
    return(~PINE);    
}

void set_leds(uint8_t new_status)
{
    PORTA=new_status;
}
