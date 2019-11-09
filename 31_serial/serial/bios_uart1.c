//* USART1 library - implementation file - 2019 *
#include "bios_uart1.h"
#include <avr/io.h>

#define xtal 16000000L

void uart1_initialize(uint16_t baud)
{
    // set up baud rate
    uint32_t temp = (xtal/16)/baud-1; // Note: UBRR can hold value 0..4095 (not up to 65535)
    UBRR1H = (temp >> 8) & 0x0F;
    UBRR1L = (temp & 0xFF);

    // enable both sending and receiving
    UCSR1B = (1<<RXEN)|(1<<TXEN);

    // Set frame format: 8 data bits, 2 stop bits, no parity
    UCSR1C = (3<<UCSZ0)|(1<<USBS);
}

void uart1_initialize3 (uint16_t baud, uint8_t uart_stop_mode, uint8_t uart_parity_mode)
{
    // set up baud rate
    uint32_t temp = xtal/16/baud-1;
    UBRR1H = (temp >> 8) & 0x0F;
    UBRR1L = (temp & 0xFF);

    // enable both sending and receiving
    UCSR1B = (1<<RXEN)|(1<<TXEN);

    // Set frame format: 8 data bits, uart_stop_mode stop bit, uart_parity_mode parity type
    UCSR1C = (3<<UCSZ0)|(1<<USBS);  // To be replaced by the line below
    // UCSR1C =  (3<<UCSZ0) | ( (uart_stop_mode & 0x01) << ___ ) | ( (uart_parity_mode & 0x03) << ___ ); // to be added
}


void uart1_shutdown ()
{
    UCSR1B = 0;
}


uint8_t uart1_ready_TX ()
{
    return ( 0 != (UCSR1A & 1<<UDRE) );
}

void uart1_putc (char c)
{
    while( 0 == (UCSR1A & 1<<UDRE) ) ;
    UDR1 = c;    
}

void uart1_puts (const char* const s)
{
    for ( const char* p = s; *p!='\0'; ++p )
        uart1_putc(*p);
}

uint8_t uart1_ready_RX ()
{
    return ( 0 != (UCSR1A & 1<<RXC) );
}

char uart1_getc ()
{
    while( 0 == (UCSR1A & 1<<RXC) ) ;
    return ( (uint8_t) UDR1 );
}

char uart1_getc_echo ()
{
   char c = uart1_getc();
   uart1_putc(c); // this is console echo for a live user
   return c;    
}

uint8_t uart1_check_error()
{
    // checks for Frame Error, Data OverRun, and Parity Error - 0b000eee0
    return( UCSR1A & ((1<<FE)|(1<<DOR)|(1<<UPE)));
}

size_t uart1_gets_echo (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size; // to allow space for '\0'

    while (1) {
        c=uart1_getc();
        if (c=='\b'||c==127) {
            uart1_putc('\a');
        } else if (c=='\n'||c=='\r') {
            uart1_putc('\r');
            uart1_putc('\n');
            *s=0;
            break;            
        } else {
            if (count<size) {
                *s++=c; // *s=c; ++s;
                ++count;
                uart1_putc(c);
            } else {
                uart1_putc('\a');
            }            
        }
    }
    return(count);
}

size_t uart1_gets (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size;

    while (1) {
        c=uart1_getc_echo();
        if ( c=='\n' || c=='\r' || count>=size )
        break;
        // return if new line, return or buffer full
        *s++=c; // *s=c; ++s;
        ++count;
    }
    *s=0; // remember to terminate the char array string
    return(count);
}

size_t uart1_gets_edit (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size; // to allow space for '\0'

    while (1) {
        c=uart1_getc();
        switch(c) {
            case '\b': // erase (^H)
            case 127: // backspace (^?)
                if (count) {
                    uart1_putc('\b');
                    uart1_putc(' ');
                    uart1_putc('\b');
                    --s;
                    --count;
                }
                break;
            case '\n':
            case '\r': // CR or LF
                uart1_putc('\r');
                uart1_putc('\n');
                *s=0;
                return count;
                default:
                if (count<size) {
                    *s++=c; // *s=c; ++s;
                    ++count;
                    uart1_putc(c);
                } else {
                    uart1_putc('\a');
                }
                break;
        }
    }
    return(count);
}

size_t uart1_write (const void* const s, size_t size)
{
    const uint8_t * p =s;
    size_t i=0;
    while ( i<size )
    {
        uart1_putc(*p);    
        ++p;
        ++i;
    }
    return(i);
}

size_t uart1_read (void* s, size_t size)
{
    uint8_t * p =s;
    char c;
    size_t count=0;

    while (count<size) {
        c=uart1_getc();
        *p++=c;
        ++count;
    }
    return(count);
}
