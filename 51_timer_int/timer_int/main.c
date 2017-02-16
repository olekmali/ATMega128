//* testing TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdlib.h>
#include <avr/interrupt.h>


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define INT_FRQ     (10)
#define KEY_MODE    (3)
#define LED_MAIN    (2)
#define LED_OUT     (0)


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    static uint8_t extended_counter = 0;
    extended_counter++;

    if ( 9==extended_counter )
    {
        led8_set( led8_get() | (1<<LED_OUT) );
    }        

    if ( 10==extended_counter )
    {
        led8_set( led8_get() & ~(1<<LED_OUT) );
        extended_counter = 0;      
    }
}


int main1(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    while (1)
    {
        cli();
        led8_set( led8_get() ^ (1<<LED_MAIN) );
        sei();
    }    

    return(0);
}


int main2(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    while (1)
    {
        // cli(); // Exam qestion: what eventually goes wrong in this case?
        led8_set( led8_get() ^ (1<<LED_MAIN) );
        // sei();
    }

    return(0);
}

int main3(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    uint8_t state = 0;
    while (1)
    {
        uint8_t new_state = key4_get() & (1<<KEY_MODE);
        if ( state != new_state && state /* !=0 */ )
        {
            cli();
            led8_set( led8_get() ^ (1<<LED_MAIN) );
            sei();
        }
        state = new_state;
        delay(20);
    }

    return(0);
}

int main4(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    uint8_t state = 0;
    while (1)
    {
        uint8_t new_state = key4_get() & (1<<KEY_MODE);
        if ( state != new_state && state /* !=0 */ )
        {
            // cli(); // Exam qestion: what eventually goes wrong in this case 
                      //               and why it is more rare than in case of main2()?
            led8_set( led8_get() ^ (1<<LED_MAIN) );
            // sei();
        }
        state = new_state;
        delay(20);
    }

    return(0);
}



int main(void)
{
    main3();

    return(0);
}
