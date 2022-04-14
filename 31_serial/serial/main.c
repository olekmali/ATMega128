//* testing USART1 - main.c *
#include "bios_uart1.h"
#include "bios_led8.h"
#include <stdlib.h>
#include <avr/io.h>
#include <string.h> // used for strlen in some tests



int main_test1_putc_connection(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        uart1_putc('A');
        delay(1000);    // remove the delay to check what is the transmission speed
                        // limited only bu the UART transmission speed
    }
    return(0);
}



int main_test2_puts(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        uart1_puts("sample text\r\n"); // experiment with the new line character(s)
                        // try \r\n - Windows, \r - MacOS, \n Unix. Linux, and nothing
                        // if interested, see also: https://en.wikipedia.org/wiki/Newline
                        // also try to put \r\n in the middle of the text
                        // also try to put \0 in the middle of the text (-- will cut off the remainder)
        delay(5000);
    }
    return(0);
}



int main_test3_write(void)
{
    const char str[]  = "sample \0text\r\n";
    const uint8_t len = sizeof(str); // you can use strlen if there is no \0 inside

    led8_init();
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        uart1_write(str, len); // binary write will write anything, including \0 in the middle
        delay(5000);
    }
    return(0);
}



int main_test4_readyRX(void)
{
    led8_init();
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        led8_set(0);
        if ( uart1_ready_RX() )
        {
            led8_set( uart1_getc() );   // display character ASCII code as number
            delay(1000);                // and keep on the display for one second

            // note that only two characters can be kept in a receiving buffer
            // while the program does not read from the UART. All else is lost
        }
    }
    return(0);
}



int main_test5_readyTX(void)
{
    led8_init();
    uart1_initialize(uart_bps_9600);

    char c='A';
    while (1)
    {
        if ( uart1_ready_TX() )
        {
            led8_set(0xFF);
            uart1_putc(c);
            if (c<'Z') ++c; else c='A';
            led8_set(0x00);
        }
    }
    return(0);
}



int main_test6_getc_echo(void)
{
    led8_init();
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        led8_set( uart1_getc_echo() );   // display character ASCII code as number
        // this time we are testing reading with an echo for a human being on the other side
    }
    return(0);
}



int main_test7_getc_puts(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[10]; // make sure that it is large enough - worst case number of digits plus one

        uint8_t b = uart1_getc(); // technically a byte is just a byte
        // but please do use uint8_t or int8_t when you consider the byte as a number
        // char may be signed or unsigned by default depending on a compiler

        uart1_puts("code: 0x");
        uart1_puts( itoa( b, buffer, 16 ) );
        uart1_puts(" which is 0b");
        uart1_puts( itoa( b, buffer, 2 ) );
        uart1_puts(" which is ");
        uart1_puts( itoa( b, buffer, 10 ) );
        uart1_putc('\r');
        uart1_putc('\n');
    }
    return(0);
}



int main_test8_gets(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[16];
       uart1_puts("Please enter some text up to 15 characters:");
        uart1_gets_echo(buffer, sizeof(buffer));
    }
    return(0);
}



int main_test9_gets_edit(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[16];
        uart1_puts("Please enter some text up to 15 characters (with editing):");
        uart1_gets_edit(buffer, sizeof(buffer));
    }
    return(0);
}



int main_test10_atoi(void)
{
    uart1_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[6];
        uart1_puts("Please enter an integer number:");
        uart1_gets(buffer, sizeof(buffer));
        uart1_puts("\r\n"); // check what happens to the printout if you remove this line

        uint16_t b = atoi(buffer);
        if (b==0) {
            uart1_puts("You entered 0 or not a number\r\n");
        } else if (b<256) {
            uart1_puts("The ASCII character for this number is '");
            uart1_putc( (uint8_t)b );// we say to the compiler: it is OK to truncate to 8 bits.
            uart1_puts("'\r\n");
        } else {
            uart1_puts("You entered a number > 255\r\n");
        }

    }
    return(0);
}



int main_testB_lower_speed(void)
{
    uart1_initialize(uart_bps_2400);

    while (1)
    {
        uart1_puts("sample text\r\n");
        delay(5000);
    }
    return(0);
}



int main_testC_higher_speed(void)
{
    uart1_initialize(uart_bps_57600);

    while (1)
    {
        uart1_puts("sample text\r\n");
        delay(5000);
    }
    return(0);
}



int main_testD_ascii_table(void)
{
    char c=' ';

    led8_init();
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        led8_set(c);
        uart1_putc(c);
        delay(1000);
        c++; // warning - uint8_t should be used instead of char
        // because we intend to roll over at 128 or 256
        // and signed char behavior is undefined at that point
    }
    return(0);
}



int main(void)
{
    return( main_test6_getc_echo() ); // replace with tests 1, 2, 3, ... C, D.
}
