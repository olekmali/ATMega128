/************************************************
	KEYPAD CONNECTIONS
*************************************************/
#define KEY_PORT PORTC		//Port C: C0-3 Columns 0-3, C4-7 Rows 0-3

// newkeypad
// 1 2 3 A					// Row 0 pin 1 keypad
// 4 5 6 B					// Row 1 pin 2 keypad
// 7 8 9 C					// Row 2 pin 3 keypad
// * 0 # D					// Row 3 pin 4 keypad
//							// Col 1 pin 5 keypad
//							// Col 2 pin 6 keypad
//							// Col 3 pin 7 keypad
//							// Col 4 pin 8 keypad

// Columns will be outputs, rows inputs
#define KEY_DIR DDRC		// Date Direction Port C
#define no_key 0x0f			// no key press
#define invalid_key 0x0ff	// invalid code