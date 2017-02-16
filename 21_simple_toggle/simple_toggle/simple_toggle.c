# include <avr/io.h>

int main (void)
{
    // set all pins on PORTD for output and switch LED them off
    DDRA  = 0xFF;
    PORTA = 0x00;

    // do it forever without any delay
    while (1)
    {
        volatile unsigned long int counter;
        for (counter=0; counter<14285; counter++) ;

        PORTA = ~PINA;
    }
    return(0);
}
