//* bootmenu.c *
#include "bootmenu.h"
#include <avr/io.h>

uint8_t boottime_menu()
{
	/* The alternative functionality - save the prior state and restore it later
	uint8_t save_dir  = DDRE;
	uint8_t save_port = PORTE;
	*/

    DDRE  = 0x00;
    PORTE = 0xF0;
	
	// wait say: 500uS for pull up resistors to recharge pins before reading them
	// note: The actual minimum is 3 cycles plus 
	//       whatever to take to charge the parasitic C
	//       using pull up resistors
	for (volatile uint8_t i=100; i!=0; ++i) ;

	uint8_t choice = ( (~PINE)>>4) & 0x0F; 
    PORTE = 0x00;

	/* The alternative to PORTE = 0 is below:
	DDRE  = save_dir;
	PORTE = save_port;
	*/

    return(choice);
}
