//* lib_kpad_test.c *
#include "lib_kpad_test.h"
#include "bios.h"
#include "bios_kpad.h"

int test1_count_up(void)
{
    init_digital_io();
    
    uint8_t i=0;
    while(1)
    {
        i++;
        display_byte(i);
        delay(100);
    }
    return(0);
}

int test2_keypad_status_low(void)
{
    init_digital_io();
    
    while(1)
    {
        uint16_t keys = scan_keypad();
        display_byte( 0xFF & keys );

    }
    return(0);
}

int test3_keypad_status_high(void)
{
    init_digital_io();

    while(1)
    {
        uint16_t keys = scan_keypad();
        display_byte( 0xFF & (keys >> 8) );

    }
    return(0);
}

int test4_keypressed_status(void)
{
    init_digital_io();
    
    while(1)
    {
        display_byte( keypressed() );

    }
    return(0);
}

int test5_getkey_value(void)
{
    init_digital_io();
    
    while(1)
    {
        display_byte( getkey() );
    }
    return(0);
}

int test6_getkey_debounce(void)
{
    init_digital_io();
    
    uint8_t buffer=0;
    while(1)
    {
        display_byte( buffer );
        buffer = buffer<<4 | getkey();
    }
    return(0);    
}

int test7_getkey_autorepeat(void)
{
    init_digital_io();
    
    uint8_t count=0;
    uint8_t lastk=0;
    while(1)
    {
        uint8_t key=getkey();
        if (key!=lastk)
        {
            lastk=key;
            count=1;
        } else {
            count++;
        }
        // display count of current key in the higher nibble
        // display the key number       in the lower  nibble
        display_byte( ( count << 4 )  |  ( key & 0x0F ) );
    }
    return(0);
}
