//* USART1 interrupt-based library - implementation file *
#include "bios_uart1_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define xtal 16000000L

static void (*UART1_RX_ISR_function)(char) = 0;

void uart1_initialize( uint16_t baud, void (*handle_rx)(char) )
{
    uint32_t temp = xtal/16/baud-1;
    UBRR1H = (temp >> 8) & 0x0F;
    UBRR1L = (temp & 0xFF);

    UART1_RX_ISR_function = handle_rx;

    // Set frame format: 8data, 2stop bit
    UCSR1C = (1<<USBS1)|(3<<UCSZ10);

    UCSR1B = (1<<RXEN1)|(1<<TXEN1) |(1<<RXCIE1);
                            //   ^^^^^^^^^^^ data received interrupt enabled (when global interrupts are enabled)
}

void uart1_shutdown ()
{
    UCSR1B = 0;
}


uint8_t uart1_ready_TX ()
{
    return ( 0 != (UCSR1A & 1<<UDRE1) );
}

void uart1_putc (char c)
{
    while( 0 == (UCSR1A & 1<<UDRE1) ) ;
    UDR1 = c;
}

void uart1_puts (const char* const s)
{
    for ( const char* p = s; *p!='\0'; ++p )
    uart1_putc(*p);
}

ISR(USART1_RX_vect)
{
    char value = UDR1;                   // read UART register into value
    UART1_RX_ISR_function(value);      // call the _SHORT_ user defined function to handle it
}
