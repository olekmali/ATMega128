//* testing TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdint.h>
#include <avr/interrupt.h>


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define INT_FRQ     (100)
// Note: use 100 for blinking, and 10,000 for dimming

#define KEY_MODE    (3)
#define LED_OUT     (0)


//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static uint8_t shared_variable = 0;


//------------------------------------------------------------------------------------
// Functions used to access the global variables shared with interrupts
//------------------------------------------------------------------------------------
void MyTimer_setParam(uint8_t new_value)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    
    shared_variable = new_value;        // disabling and enabling
    // interrupts is actually needed only if you have more than one byte to communicate
    
    SREG = sreg_save;                   // restore interrupts
}


uint8_t MyTimer_getParam(void)
{
    uint8_t value;
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    
    value = shared_variable;            // disabling and enabling
    // interrupts is actually needed only if you have more than one byte to communicate and interrupt modifies it
    
    SREG = sreg_save;                   // restore interrupts
    return(value);
}


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    const static uint8_t constants[] = { 0, 25, 50, 75, 100 };
    static uint8_t counter = 0;
    static uint8_t curmode = 0;
    
    if (counter>0)
    {
        counter--;
    } 
    else
    {
        counter = 100;
        curmode = shared_variable; // update at the beginning of a new cycle only
    }

    if ( counter>= constants[curmode] )
    {
        led8_set( led8_get() & ~(1<<LED_OUT) );
    }
    else
    {
        led8_set( led8_get() | (1<<LED_OUT) );
    }
}


int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_8 );
    // Exam question: why the prescaler of 8 was used for interrupt frequency of 100Hz and xtal of 16MHz?
    // Exam question: why the prescaler of 1 was used for interrupt frequency of 10kHz and xtal of 16MHz?

    uint8_t mode = 1;
    MyTimer_setParam(mode);

    sei();


    uint8_t state = 0;
    while (1)
    {
        uint8_t new_state = key4_get() & (1<<KEY_MODE);
        if ( state != new_state && state /* !=0 */ )
        {
            mode++;
            if (mode>4) mode=0;

            MyTimer_setParam(mode);
        }
        state = new_state;
        delay(20);
    }
        
    return(0);
}
