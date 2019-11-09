//* testing USART1 - main.c *
#include "bios_uart1.h"
#include "bios_led8.h"
#include <stdlib.h>
#include <avr/io.h>


int main(void)
{
    char c='A';
    led8_init();
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        if ( uart1_ready_TX() ) // replace the condition with true and compare
        {
            led8_set(0xFF);
            uart1_putc(c);
            if (c<'Z') ++c; else c='A';
            led8_set(0x00);
        }
    }
    return(0);
}



/*

int main_test1_putc_connection(void)
{
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_putc('A');
    }
    return(0);
}

int main_test2_puts(void)
{
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_puts("sample text\r\n");
    }
    return(0);
}


#include <string.h>
int main_test3_write(void)
{
    const char* str = "sample text\r\n";
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_write(str, strlen(str));
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
            led8_set( uart1_getc() );
            delay(1000);
        }
    }
    return(0);
}


int main_test5_readyTX(void)
{
    char c='A';
    led8_init();
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        if ( uart1_ready_TX() ) // replace the condition with true and compare
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
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_getc_echo(); // ignore the returned value
    }
    return(0);
}


int main_test7_getc_puts(void)
{
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        char buffer[4];
        char c = uart1_getc();
        uart1_puts("code: 0x");
        uart1_puts( itoa( c, buffer, 16 ) );
        uart1_puts(" which is ");
        uart1_puts( itoa( c, buffer, 10 ) );
        uart1_putc('\r');
        uart1_putc('\n');
    }
    return(0);
}


int main_test8_gets(void)
{
    char buffer[16];
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_puts("Please enter some text up to 15 characters:");
        uart1_gets_echo(buffer, sizeof(buffer));
    }
    return(0);
}


int main_test9_gets_edit(void)
{
    char buffer[16];
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        uart1_puts("Please enter some text up to 15 characters (with editing):");
        uart1_gets_edit(buffer, sizeof(buffer));
    }
    return(0);
}


int main_testA_higher_speed(void)
{
    uart1_initialize(uart_bps_57600);
    while (1)
    {
        uart1_putc('A');
    }
    return(0);
}

int main_test11(void)
{
    char c='A';
    led8_init();
    uart1_initialize(uart_bps_9600);
    while (1)
    {
        if ( uart1_ready_TX() ) // replace the condition with true and compare
        {
            led8_set(0xFF);
            uart1_putc(c);
            if (c<'Z') ++c; else c='A';
            led8_set(0x00);
        }
    }
    return(0);
}



*/
