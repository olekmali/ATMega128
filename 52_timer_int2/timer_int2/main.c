//* testing TIMER1 interrupt used to synchronize events in main loop - main.c *
#include "bios_timer_int.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdint.h>
#include <avr/interrupt.h>


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define INT_FREQUENCY       100


//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t     semaphore = 0;  // volatile keyword is very important here!


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // Remember: we are inside the interrupt routine
    //              - no excessive loops
    //              - no time consuming tasks
    //              - no interrupt enabling

    semaphore = 1;
}


int main(void)
{
    led8_init();
    led8_set(0);
    key4_init();

    Timer1_initialize( INT_FREQUENCY, MyTimerFN, timer_prescale_8 );
    // Exam question: why the prescaler of  1 was used for interrupt frequency of 10kHz and xtal of 16MHz?
    // Exam question: why the prescaler of  8 was used for interrupt frequency of 100Hz and xtal of 16MHz?
    // Exam question: why the prescaler of 64 was used for interrupt frequency of  10Hz and xtal of 16MHz?


    sei();


    uint8_t prevbut = 0;
    uint8_t rate0 = 2;
    uint8_t rate4 = 8;
    uint8_t count = 0;
    while (1)
    {
        // timing the loop section
        while( semaphore!=1 )   // wait for the time marked by the Timer interrupt:
            ; // semaphore will be set to 1 when the Timer interrupt calls MyTimerFN()
        semaphore = 0;          // prepare for another wait

        // debugging if the interrupt controls the timing of the loop:
        // led8_set( led8_get() ^ 0b10000000 );

        // timing the events in the loop section
        if (count<9) {
            count++;
        } else {
            count = 0;
            // debugging the timing in the loop:
            // led8_set( led8_get() ^ 0b01000000 );
        }

        // action section
        uint8_t curLEDs = led8_get();
        if ( count<rate0 ) {
            curLEDs = curLEDs | 0b00000001; // in class we picked a different LED here
        } else {
            curLEDs = curLEDs & 0b11111110;
        }
        if ( count<rate4 ) {
            curLEDs = curLEDs | 0b00010000; // in class we picked the LED4 for the second LED to control
            } else {
            curLEDs = curLEDs & 0b11101111;
        }
        led8_set(curLEDs);

        // responding to input section
        uint8_t curbut = key4_get();
        uint8_t chgbut = ( prevbut^curbut ) & curbut;
        prevbut = curbut; // important! we always compare the current state with the one that just passed

        if ( chgbut & 0b00000001 ) { // detecting if the button K4 was just pressed
            if (rate0<10) rate0 = rate0+2;
            else rate0 = 0;
        }

        if ( chgbut & 0b0000100 ) { // detecting if the button K6 was just pressed
            if (rate4<10) rate4 = rate4+2;
            else rate4=10;
        }

        if ( chgbut & 0b0001000 ) { // detecting if the button K7 was just pressed
            if (rate4>2) rate4 = rate4-2;
            else rate4=0;
        }

    }

    return(0);
}
