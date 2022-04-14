//* blink and toggle in c - test1.c *
#include "bios.h"
// #include "library.h"

// This test should work out of the box without your library
int main(void)
{
    init_digital_io();

    while(1)
    {
        set_leds( get_buttons() );
    }

    return(0);
}
