/*
 * wavshare_bd_test.c
 *
 * Created: 7/15/2012 1:43:58 PM
 * Revised 7/17/12 by gld
 *  Author: gld
 * created from motor_control_712,c, see below
 * delete unused functions from motor_control_712.c
 */ 

/*
 * motor_control_712.c, G. Dempsey 7/12/12
 * adjust for new Bradley kit by Chris Mattus
 * remapped LCD to Port A: update lcd.h and motor.h (rename board.h), clear LCD R/W at startup
 *
 * Created: 6/13/2012 12:45:46 PM
 * G. Dempsey
 * Move LCD strobe from PD7 to PD6 since PD7 will be used for rotory encoder input.
 * motor_control_613 developed from motor_drive_529 (AVR 6.0 Project)
 * optimization level = 0 (none)
 * May 29, 2012
 * G. Dempsey May 23, 2012, modified pan_tilt 5/16 software for closed-loop motor control
 * original motor code also used from 2011 Bradley mini project (80535 device)
 * had to change delay reloads since new compiler generates faster code
 * modified LCD code for Atemga128 from Author: Avinash Gupta
 * http://extremeelectronics.co.in/avr-tutorials/using-lcd-module-with-avrs/
 * Dempsey: eliminated read functions (R/Wbar tied to 0Volts)
 * No busy read operations: wait for specified delays
 * optimized for minimal execution time: separated command and character write operations
 *  
 */

// 16MHz Crystal, instruction cycle 62.5ns

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pgmspace.h"
#include <inttypes.h>
#include "lcd.h"						// LCD setup
#include "keypad.h"						// Keypad setup
#include "board.h"						// Board Setup: Definitions and Functions

const unsigned char keytext[20] PROGMEM ="147F2580369EABCD";	// keypad map, E=#, F=*

unsigned char gen_cnt,delay_us_ctr, delay_ms_ctr, key,last_key,key_scan;
unsigned char switch_data, key_state, tstcmd; /* 1 byte */
unsigned char key_inv; // byte but use as flag (0 or 1)

int main(void)
{
										// start hardware initialization */
	delay_ms_ctr=ms_250;				// delay 250ms, wait for supply stabilization
	delay_ms();

	init_timer_ports();					// setup timers, interrupts, ports
	key_init();							// initialize Keypad
	prog_init();						// start program initialization */
	display_init();						// initial start screen
	sei ();								// Enable global interrupts, then enter main loop

    while(1)
    {
	if(tstcmd==0) lcd_test();			// LCD display test
	if(tstcmd==1) keypad_test();		// Keypad test
	if(tstcmd==2) led_test();			// LED test: connect Ports B, E, F0-3, G0-4 to LEDS with cable
	if(tstcmd==3) switch_test();		// On-board Switch Test: Port D to switches with cable
	LCDGotoXY(0,0);						// cursor home, repeat tests
    }
}

void LCDByte(uint8_t c,uint8_t isdata)
{
//Sends a byte to the LCD in 4bit mode
//cmd=0 for data
//cmd=1 for command

uint8_t hn,ln;							// High and Low Nibbles
uint8_t temp;

	hn=c>>4;
	ln=(c & 0x0F);

	if(isdata==0) CLEAR_RS();			// RS= 0, LCD command
	else
	SET_RS();							// RS = 1, LCD data
	delay_us_ctr=us_2;					// tAS + some margin 
	delay_us();
	SET_E();
										// Send high nibble
	temp=(LCD_DATA_PORT & 0XF0)|(hn);
	LCD_DATA_PORT=temp;
	delay_us_ctr=us_2;					// teH	+ margin
	delay_us();
	SET_E();							// Now data lines are stable pull E low for transmission
	CLEAR_E();
	delay_us_ctr=us_2;
	delay_us();
										//Send the lower nibble
	SET_E();
	temp=(LCD_DATA_PORT & 0XF0)|(ln);
	LCD_DATA_PORT=temp;
	delay_us_ctr=us_2;					// tEH
	delay_us();	
	CLEAR_E();							// SEND

	if(isdata==0)
	{
		delay_ms_ctr=ms_2;				// delay 2ms if LCD command
		delay_ms();
	}
	if (isdata==1)
	{
		delay_us_ctr=us_50;				// delay 50us if LCD data
		delay_us();
	}	
}

void InitLCD(uint8_t style)
{
	/*****************************************************************
	
	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK :The cursor is blinking type
	LS_ULINE :Cursor is "underline" type else "block" type

	*****************************************************************/
	
	//After power on Wait for LCD to Initialize
	delay_ms_ctr=ms_50;	
	delay_ms();
	
	//Set IO Ports
	LCD_DATA_DDR|=(0x0F);
	LCD_E_DDR|=(1<<LCD_E_POS);
	LCD_RS_DDR|=(1<<LCD_RS_POS);		//Port G4
										//R/W is tied low, LCD_RW_DDR|=(1<<LCD_RW_POS);
	LCD_DATA_PORT&=0XF0;
	CLEAR_E();							//Port D7
	delay_ms_ctr=ms_1;	
	delay_ms();
	CLEAR_RS();							// RS low for command
	
	delay_ms_ctr=ms_1;	
	delay_ms();	
										// write "3" three times to command register
	SET_E();							// E= 1 to E= 0 to strobe in data
	LCD_DATA_PORT|=(0b00000011);	
	delay_ms_ctr=ms_1;	
	delay_ms();							// 1ms delay for first "3"
	CLEAR_E();
	delay_ms_ctr=ms_5;	
	delay_ms();		
	
	SET_E();							// E= 1 to E= 0 to strobe in data
	LCD_DATA_PORT|=(0b00000011);	
	delay_us_ctr=us_50;					// 50 us for second "3"
	delay_us();	
	CLEAR_E();
	delay_ms_ctr=ms_1;	
	delay_ms();		

	SET_E();							// E= 1 to E= 0 to strobe in data
	LCD_DATA_PORT|=(0b00000011);	
	delay_us_ctr=us_50;					// 60us for second "3"
	delay_us();	
	CLEAR_E();
	delay_ms_ctr=ms_1;	
	delay_ms();	

	LCD_DATA_PORT&=0XF0;
										//Set 4-bit mode
	delay_ms_ctr=ms_1;					//tAS, add 700ns more	
	delay_ms();	
	SET_E();							// E= 1 to E= 0 to strobe in data
	LCD_DATA_PORT|=(0b00000010);		//write "2" for 4-bit mode
	delay_ms_ctr=ms_1;					//tAS, add 700ns more	
	delay_ms();	
	CLEAR_E();
										//Wait for LCD to execute instead of busy check 
	delay_ms_ctr=ms_100;				// delay 1000ms instead of checking for busy
	delay_ms();	
										//Now the LCD is in 4-bit mode
	LCDCmd(0b00101000);					//function set 4-bit,2 line 5x7 dot format
	LCDCmd(0b00001100);					//Display On cursor off
	LCDCmd(0b00000110);					// auto increment
	LCDCmd(0b10000000);					// cursor to line 1
	LCDCmd(0b00001100|style);			//Display On
}

void LCDWriteCharacter(char cmsg)
{
	/*****************************************************************
	
	This function Writes a single character to lcd at the current cursor
	location.

	Arguments:
	msg: character


	*****************************************************************/
	LCDData(cmsg);
}
	
void LCDWriteString(const char *msg)
{
	/*****************************************************************
	
	This function Writes a given string to lcd at the current cursor
	location.

	Arguments:
	msg: a null terminated string to print


	*****************************************************************/
 while(*msg!='\0')
 {
	LCDData(*msg);
	msg++;
 }
}

void LCDWriteInt(int val,unsigned int field_length)
{
	/***************************************************************
	This function writes a integer type value to LCD module

	Arguments:
	1)int val	: Value to print

	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val

	****************************************************************/

	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) LCDData('-');
	for(i=j;i<5;i++)
	{
	LCDData(48+str[i]);
	}
}
void LCDGotoXY(uint8_t x,uint8_t y)
{
 if(x<40)
 {
  if(y) x|=0b01000000;
  x|=0b10000000;
  LCDCmd(x);
  }
}

void keypad(void)
{
//const unsigned char keytext[20] PROGMEM ="CDEF369B2580147A";
unsigned char key_temp;
unsigned int i;

	key_scan=no_key;					// initialize key_scan to no key
	KEY_DIR=0;							// Setup Port C4-7 Input: use as outputs by tristating and change
										// to 0V by setting DDR bit	scan column 0, Port C0
	KEY_DIR |= (1 << 0);	
	delay_us_ctr=us_10;
	delay_us();	
	
	key_temp=PINC;
	
	key_temp = key_temp>>4;				// move to low nibble
	if (key_temp!=no_key)
	{
	key_temp = ~key_temp;				// make key press a "1"
	key_temp &= 0X0F;					// leave low nibble
										// convert to ASCII
		for (i=0;i<4;i++)
		{
		if(key_temp & (1<<i)) key_scan=pgm_read_byte(&keytext[i]);
		}
	}
										// scan column 1, Port C1
	KEY_DIR=0;	
	KEY_DIR |= (1 << 1);	
	delay_us_ctr=us_10;	
	delay_us();	
	
	key_temp=PINC;						// read row 0, Port C4-7
	key_temp = key_temp>>4;				// move to low nibble
	if (key_temp!=no_key)
	{
	key_temp = ~key_temp;				// make key press a "1"
	key_temp &= 0X0F;					// leave low nibble
										// convert to ASCII
		for (i=0;i<4;i++)
		{
		if(key_temp & (1<<i)) key_scan=pgm_read_byte(&keytext[i+4]);
		}
	}
										// scan column 2, Port C2
	KEY_DIR=0;
	KEY_DIR |= (1 << 2);	
	delay_us_ctr=us_10;
	delay_us();	
	
	key_temp=PINC;						// read row 0, Port C4-7
	key_temp = key_temp>>4;				// move to low nibble
	if (key_temp!=no_key)
	{
	key_temp = ~key_temp;				// make key press a "1"
	key_temp &= 0X0F;					// leave low nibble
										// convert to ASCII
		for (i=0;i<4;i++)
		{
		if(key_temp & (1<<i)) key_scan=pgm_read_byte(&keytext[i+8]);
		}
	}
										// scan column 3, Port C3
	KEY_DIR=0;	
	KEY_DIR |= (1 << 3);	
	delay_us_ctr=us_10;
	delay_us();	
	
	key_temp=PINC;						// read row 0, Port C4-7
		
	key_temp = key_temp>>4;				// move to low nibble
	if (key_temp!=no_key)
	{
	key_temp = ~key_temp;				// make key press a "1"
	key_temp &= 0X0F;					// leave low nibble
										// convert to ASCII
		for (i=0;i<4;i++)
		{
		if(key_temp & (1<<i)) key_scan=pgm_read_byte(&keytext[i+12]);
		}
	}	
										// now check keypad debounce states
										// key_scan has current key
	for (i=0;i<4;i++)
	{
		
	
	switch(key_state)
	{

	/* idle/power up state */
	case 0:
	if (key_scan!=no_key)					/* check for new key */
		{
		last_key = key_scan;				/* setup for next state */
		key_state++;
		gen_cnt=ms_5;
		delay();	
		}
	break;

	case 1:
	if (key_scan != last_key)
		{
		key_state=0;						/* return to idle */
		last_key=no_key;
		}		
	else
		{
		key_state++;						/* next state and wait for key release */
		gen_cnt=ms_5;
		delay();	
		}
	break;

	case 2:
	if (key_scan == no_key)			;		// key debounced, go to next state and wait for main loop to process,
											//else stay here/
		{
		key_state++;
		key=last_key;						// update key for Main Loop
		gen_cnt=ms_5;
		delay();	
		}
	break;

	case 3:
	if (key== invalid_key)key_state=0;		/* key processed? Then go back to idle next time, else stay here */
	break;
	}	

	}										// end for loop
	KEY_DIR=0;								// leave low nibble tristate
}

void key_init(void)
{
	
KEY_DIR=0;								// Setup Port C4-7 Input: use as outputs by tri-stating and change to outputs (0V) by setting DDR bit
										// and Setup Port C0-3 as inputs with internal pullup

KEY_PORT &= ~0x0F;						// write "0" to tristate the columns
KEY_PORT |= 0x0F0;;						// write "1" to pullup the rows
key=0xff;								// initialize to invalid code
last_key=key;

}

void delay(void)
{
while(1)
	{
	if (gen_cnt==0)	break;				// check for timeout*/
	}
}

void init_timer_ports(void)
{
	DDRA |= _BV(DDB5);					// define Port A bit 5 as output
	CLEAR_RW();							// keep bit 5 LCD R/W low forever: only use write mode

										// setup timer 0 for general purpose 1.024ms timer
										// Timer 0 has lower priority than Timer 1 (PWM functions)
										// Mode 0: rolls over and keeps going
										// 1.024ms: prescale 64 (16us clock), then count 64*16us = 1.024ms
	TCCR0|= (1 << CS02 );				// divide by 64
	TCCR0|= (0 << CS01 );				// divide by 64
	TCCR0|= (0 << CS00 );				// divide by 64
	TIMSK |= (1 << TOIE0 );				// Enable overflow interrupt for timer 0
	//
										//// Timer 1 (64 us period)
////	DDRB |= _BV(DDB6);					// OC1B as PWM output Port B bit 6, cleared on count up, set on count down
	//ICR1 = 1024;						// setup for 64us period, 15,625Hz PWM freq
////	TCCR1A |= _BV(COM1B1);				// Clear OC1B on compare match: set to zero volts
	//TCCR1A |= _BV(WGM10)| _BV(WGM11);	// mode 7, PWM, Fast PWM 10 bit (TOP value is 1023)
	//TCCR1B |= _BV(WGM12);				// mode 7, PWM, Fast PWM 10 bit (TOP value is 1023)
////	OCR1B = 0;							// PWM 0%
	//TCCR1B |= (1 << CS10 );				// Start up timer 1 to count CPU clock rate- no prescaler
//
////	DDRB |= _BV(DDB0);					// Set LED as output on Port B bit 0 (1ms timing)
////	PORTB |= (1 << 0);					// Turn LED on	
}

void prog_init(void)
{
	tstcmd=0;							// default: LCD test
	key=invalid_key;					// initialize key to invalid code */
	key_state=0;						// Key debounce state = idle

										// Initialize LCD module
	InitLCD(LS_ULINE);					// underline cursor
	delay_ms_ctr=ms_100;				// delay 100ms, instead of checking for busy
	delay_ms();
	LCDClear();							//Clear the screen, cursor home
}

void display_init(void)
{	
										// note 4 line display, but 2 line internal for software: see lcd.h
	LCDWriteString("Bradley Univ.");	// initialize display
	LCDGotoXY(0,1);						// cursor HOME, line 2 */
	LCDWriteString("ECE Department");
	LCDGotoXY(16,0);					// line 1, locate cursor
	LCDWriteString("Test Software");
	LCDGotoXY(16,1);					// line 2, locate cursor
	LCDWriteString("Rev. 1, 7/17/12");
		
	delay_2sec();						// hold for 4 seconds
	delay_2sec();
	LCDClear();	

	LCDWriteString("Test Software");	// initialize display
	LCDGotoXY(0,1);						// cursor HOME, line 2 */
	LCDWriteString("Based on Ext.");
	LCDGotoXY(16,0);					// line 1, locate cursor
	LCDWriteString("16 MHz Crystal");

	delay_2sec();						// hold for 4 seconds
	delay_2sec();
	LCDClear();
}

void delay_us(void)						// delay approx: 0.655 us * count
{										// count =1 delay 2.13 us, count=255 delay = 161 us
while (delay_us_ctr!=0)	delay_us_ctr--;
}

void delay_ms(void)						// delay approx: 1ms per count, 0.98ms for count=1
{
unsigned int i;

	while (delay_ms_ctr!=0)
		{
		delay_ms_ctr--;
			for (i=0;i<21;i++)
			{
			delay_us_ctr=us_50;
			delay_us();	
			}
		} 
}

void delay_1sec(void)
{
	delay_ms_ctr=ms_250;				// hold for 2 seconds
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
}

void delay_2sec(void)
{
	delay_ms_ctr=ms_250;				// hold for 2 seconds
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
	delay_ms_ctr=ms_250;
	delay_ms();
	delay_ms_ctr=ms_250;	
	delay_ms();
	delay_ms_ctr=ms_250;	
	delay_ms();
	delay_ms_ctr=ms_250;	
	delay_ms();
	delay_ms_ctr=ms_250;	
	delay_ms();	
}

void lcd_test(void)
{
	LCDWriteString("LCD Test:");	
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Display all");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Character");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("Locations");
	

	delay_2sec();						// hold for 4 seconds
	delay_2sec();
	LCDClear();	
	
	LCDWriteString("ABCDEFGHIGKLMNOP");
	LCDGotoXY(0,1);						// move to line 2 */	
	LCDWriteString("QRSTUVWXYZ123456");	
	LCDGotoXY(16,0);					// move to line 1, locate cursor */	
	LCDWriteString("7890abcdefghijkl");		
	LCDGotoXY(16,1);					// move to line 2, locate cursor */	
	LCDWriteString("mnopqrstuvwxyz#*");												

	delay_2sec();						// hold for 6 seconds
	delay_2sec();
	delay_2sec();
	LCDClear();
	tstcmd=1;							// go to next test
																					
}

void keypad_test(void)
{
unsigned char key_ctr;

	LCDWriteString("Keypad Test:");		// Keypad Test */
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Press each key,");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Key --> LCD");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("# --> Ends Test");
	

	delay_2sec();						// hold for 6 seconds
	delay_2sec();
	delay_2sec();
	LCDClear();

	key_inv=0;							// clear invalid key flag */
	key_ctr=0;							// reset key ctr

	while(1)
	{
		
		while(1)
		{
			key=invalid_key;				// load invalid code */
			keypad();
			if(key>=0x30 && key<=0x39) break;
			if(key>=0x41 && key<=0x46) break;
		}

		if(key==0x45) key= 0x23;			// # key
		if(key==0x46) key= 0x2A;			// * key
		LCDWriteCharacter(key);				// display key */
		delay_ms_ctr=ms_250;
		delay_ms();
											
		if (key_ctr==15)
		{
		key_ctr=0;
		LCDClear();
		}
		else key_ctr++;
		if(key==0x23)					// terminate if # key
		{
		delay_ms_ctr=ms_250;
		delay_ms();
		LCDClear();
		break;
		}		
	}		

	LCDWriteString("Keypad Test");		// Keypad Test Terminated
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Terminated with");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("# Key");


	delay_2sec();						// hold for 4 seconds
	delay_2sec();
	LCDClear();
	tstcmd=2;							// go to next test
											
}

void led_test(void)
{
unsigned char led_ctr;

	LCDWriteString("LED Test:");		// LED Test */
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Requires Port B");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Connection Cable");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("PB0 --> LED0, ..");

	delay_2sec();						// hold for 6 seconds
	delay_2sec();
	delay_2sec();
	LCDClear();

	LCDWriteString("Can also replace");	// LED Test */
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Port B with Ports");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("E,F0-3,G0-4 for");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("additional tests");

	delay_2sec();						// hold for 6 seconds
	delay_2sec();
	delay_2sec();
	LCDClear();

	LCDWriteString("LEDs Show 8-bit");	
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Counter Results");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Then LED Test");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("is Terminated");
	
	delay_2sec();						// hold for 4 seconds
	delay_2sec();

	DDRB = 0xff;						// PORT B for Output
	PORTB = 0xff;						// Turn LEDs on initially

	DDRE = 0xff;						// PORT E for Output
	PORTE = 0xff;						// Turn LEDs on initially

	DDRF = 0xf;							// PORT F0-3 for Output
	PORTF = 0xf;						// Turn LEDs on initially

	DDRG = 0x1f;						// PORT G0-4 for Output
	PORTG = 0x1f;						// Turn LEDs on initially
	
	delay_1sec();
	PORTB = 0;							// Turn LEDs off
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;
	
	delay_1sec();
		
	for(led_ctr=0;led_ctr<255;led_ctr++)
	{
		PORTB=led_ctr;
		PORTE=led_ctr;
		PORTF=led_ctr;
		PORTG=led_ctr;
		delay_ms_ctr=ms_50;
		delay_ms();	
	}

	PORTB = 0;							// Turn LEDs off
	PORTE = 0;
	PORTF = 0xf0;
	PORTG = 0xE0;
	LCDClear();
	tstcmd=3;							// go to next test
}

void switch_test(void)
{

	LCDWriteString("Switch Test:");		// On-board Switch Test */
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Requires Port D");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Connection Cable");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("PD0 --> SW0, ..");

	delay_2sec();						// hold for 6 seconds
	delay_2sec();
	delay_2sec();
	LCDClear();

	LCDWriteString("Switch Press:");
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("SW0 --> LED0");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("Keypad #");
	LCDGotoXY(16,1);					// move to line 2, locate cursor */
	LCDWriteString("Ends Switch Test");

	delay_2sec();						// hold for 4 seconds
	delay_2sec();

	LCDClear();
	LCDWriteString("Start Testing");
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("# Terminates");

	DDRD = 0;							// PORT D for Input
	PORTD = 0xff;						// Set Port pins to "1", activates pullup (closed switch will pull down)
	PORTB = 0;							// Turn LEDs off
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;

	delay_1sec();

	while(1)
	{
		key=invalid_key;				// load invalid code */
		keypad();
		if(key==0x45) break;			// exit if # key
		switch_data=PIND;				// read switches
		PORTB=~switch_data;
		PORTE=~switch_data;
		PORTF=~switch_data;
		PORTG=~switch_data;
		delay_ms_ctr=ms_250;
		delay_ms();		
	}		
	LCDClear();
	LCDWriteString("Switch Test");		// Switch Test Terminated
	LCDGotoXY(0,1);						// move to line 2 */
	LCDWriteString("Terminated with");
	LCDGotoXY(16,0);					// move to line 1, locate cursor */
	LCDWriteString("# Key");


	delay_2sec();						// hold for 4 seconds
	delay_2sec();
	LCDClear();
	tstcmd=0;							// go to next test: back to zero

}

ISR ( TIMER0_OVF_vect )					// general purpose 1.024ms timer
{
	if (gen_cnt!=0) gen_cnt--;			// decrement general purpose ctr if non-zero
}