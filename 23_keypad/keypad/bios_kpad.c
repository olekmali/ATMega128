//* bios_kpad.c *
#include "bios_kpad.h"
#include <avr/io.h>

#define KEY_BOUNCE_CNT (5)

inline void delay_digital_io_change(void)
{
    // Note: asm("nop"); - this would work too but that is not ANSI C90 or C99 standard notation
    __asm__ __volatile__("nop");
    __asm__ __volatile__("nop");
    __asm__ __volatile__("nop");
    // Possible test question: what is inline modifier doing?
    // Possible test question: does inline work both in Debug and Release compiler mode?
    // Possible test question: what is volatile modifier doing?
    // Possible test question: does volatile work both in Debug and Release compiler mode?
}


uint16_t scan_keypad()
{
    uint16_t status = KEYPAD_NONE;

    // The scanning sequence is as follows:
    //   DDRC     &    PORTC
    // 1111 1111  &  1111 1111 -- initial charge of parasitic Cs
    // then wait for a very short time
    // 0001 0000  &  1110 1111 -- scan the bottom row
    // then wait 3+ clock cycles and read PINC & 0x0F
    // 0010 0000  &  1101 1111 -- scan the 2nd row
    // then wait 3+ clock cycles and read PINC & 0x0F
    // 0100 0000  &  1011 1111 -- scan the 3rd row
    // then wait 3+ clock cycles and read PINC & 0x0F
    // 1000 0000  &  0111 1111 -- scan the top row
    // then wait 3+ clock cycles and read PINC & 0x0F
    // 0000 0000  &  0000 0000 -- disconnect for PIN safety
 // or 1111 0000  &  1111 0000 -- for PIN change interrupt driven keypad

    DDRC  = 0xFF;                   // make all pins as output pins for a short time before the first scan 
    PORTC = 0xFF;                   // set all pins to high voltage to charge all parasitic capacitance
    // delay_digital_io_change();

    uint8_t  swip_bit = 0b00010000; // perhaps this variable could be useful to you?
    for(uint8_t i=0; i<4; i++)
    {
        DDRC  = swip_bit;           // set input/output direction for this row reading
        PORTC = ~swip_bit;          // set the active low line with all others high or with pull up resistors
        delay_digital_io_change();
        status = (status<<4) | (0x0F & ~PINC ); // read and accumulate the 4 input bits of columns
        PORTC = 0xFF;               // set the active line to 1 to quickly recharge the parasitic capacitance
        // delay_digital_io_change();
        swip_bit = swip_bit<<1;     // perhaps this variable could be useful to you?
    }

    DDRC  = 0;                      // shut down output for safety (no high current in case of pin shorting)
    PORTC = 0;                      // shut down pull ups (potentially for energy conservation)
    // note: these two lines should be set up differently for lowering edge interrupt mode in Junior Lab

    return(status);
}

char keypressed(void)
{
    static const char matrix[16] = {KEY_S, KEY_0, KEY_H, KEY_D, KEY_7, KEY_8, KEY_9, KEY_C, KEY_4, KEY_5, KEY_6, KEY_B, KEY_1, KEY_2, KEY_3, KEY_A};
    // Possible test question: what is the use for the lookup table above 
    uint8_t  keycnt = 0;
    uint8_t  keymin = KEY_NONE;
    uint16_t keys = scan_keypad();
    if (keys!=KEYPAD_NONE) {
        for (uint8_t i=0; i<16; i++) {
            if ( keys & 0x01 /* !=0 */ ) {
                keymin=matrix[i];
                keycnt++;
            }
            keys = keys >> 1;
        }
    }    
    if (keycnt<2)   return(keymin);
    else            return(KEY_MANY);
    // Possible test question: what happens if we always return(keymin) regardless of keycnt value?
}

char getkey(void)
{
    uint8_t key_current=KEY_NONE, key_old;
    uint8_t key_cnt=0;
    do {
        key_old     = key_current;
        key_current = keypressed();
        if (key_current==key_old) key_cnt++;
        else key_cnt=0;
    } while (key_current==KEY_NONE||key_current==KEY_MANY||key_cnt<KEY_BOUNCE_CNT);

    // wait for the key release
    // Challenge Question 1: How to implement simplified auto-repeat? (easy)
    // Challenge Question 2: How to implement auto-repeat with the first key wait longer, 
    //              and subsequent shorter like in the real computer keyboard? (require static (hidden global) variable inside the function)
    while (KEY_NONE!=keypressed()) ;

    return(key_current);
}
