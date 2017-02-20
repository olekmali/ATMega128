#include "myutils.h"
//
#ifndef _LCD_H
#define _LCD_H
/*_________________________________________________________________________________________*/

/************************************************
	LCD CONNECTIONS
*************************************************/

#define LCD_DATA A			//Port PA0-PA3 are connected to D4-D7

#define LCD_E A 			//Enable OR strobe signal
#define LCD_E_POS	PA4		//Position of enable in above port

#define LCD_RS A	
#define LCD_RS_POS 	PA6

#define LCD_RW A
#define LCD_RW_POS	PA5

//************************************************

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010

/***************************************************
			F U N C T I O N S
****************************************************/
void InitLCD(uint8_t style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,unsigned int field_length);
void LCDGotoXY(uint8_t x,uint8_t y);
void LCDWriteCharacter(char cmsg);

//Low level
void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define LCDClear() LCDCmd(0b00000001)
#define LCDHome() LCDCmd(0b00000010);		// takes 1.52 ms to execute

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif





