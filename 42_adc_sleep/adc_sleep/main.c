//* testing USART1 - main.c *
#include "bios_adc_sleep.h"
#include "bios_led8.h"
#include "bios_key4.h"
#include <stdlib.h>
#include <avr/interrupt.h>


int main(void)
{
    led8_init();
    key4_init();
    adc_init_wslepp();
    adc_select_ch(0);
    sei();  // enable interrupts - interrupt wakes up the MCU from sleep while reading ADC
    while (1)
    {
        if ( key4_get() & B_K7 ) {
            adc_select_ch(ADC_CH0);
        } else {
            adc_select_ch(ADC_CH1);
        }        

        if ( key4_get() & B_K4 ) {
                uint8_t val = adc_get8b();
                uint8_t disp= 0;
                while ( val>0 ) {
                    disp = ( disp << 1 ) | 1 ;
                    val >>= 1; // i.e. val = val >> 1; or val = val/2
                } 
                led8_set( disp );        
        } else {
            if ( key4_get() & B_K6 ) {
                // display only the 8LSB of 10 to check for noise
                led8_set( (uint8_t)( adc_get12b() >> 4 ) ); 
            } else {
                // display only the 8MSB
                led8_set( adc_get8b() );
            }
        }                         




    }
    return(0);
}
