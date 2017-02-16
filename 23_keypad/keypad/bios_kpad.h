//* bios_kpad.h *
#ifndef BIOS_KPAD_H_
#define BIOS_KPAD_H_
#include <stdint.h>

/*  NOTE! NEW WIRING:
    Connect the LED bar to PortA (LED0 to PortA.0)
    Connect the Keypad  to PortC (white bits 0..3, green bits 4..7)
    Connect the Buttons to PortE (upper 4 keys to upper 4 bits)
*/

// include the line below before #include "bioskpad.h" to use ASCII code
// #define KEY_ASCII 1

// *** keypressed *** return codes ***
#ifdef KEY_ASCII
    #define KEY_0           ('0')
    #define KEY_1           ('1')
    #define KEY_2           ('2')
    #define KEY_3           ('3')
    #define KEY_4           ('4')
    #define KEY_5           ('5')
    #define KEY_6           ('6')
    #define KEY_7           ('7')
    #define KEY_8           ('8')
    #define KEY_9           ('9')
    #define KEY_A           ('A')
    #define KEY_B           ('B')
    #define KEY_C           ('C')
    #define KEY_D           ('D')
    #define KEY_H           ('#')
    #define KEY_S           ('*')
    #define KEY_NONE        (0xFF)
    #define KEY_MANY        (0xF0)
#else
    #define KEY_0           (0)
    #define KEY_1           (1)
    #define KEY_2           (2)
    #define KEY_3           (3)
    #define KEY_4           (4)
    #define KEY_5           (5)
    #define KEY_6           (6)
    #define KEY_7           (7)
    #define KEY_8           (8)
    #define KEY_9           (9)
    #define KEY_A           (10)
    #define KEY_B           (11)
    #define KEY_C           (12)
    #define KEY_D           (13)
    #define KEY_H           (14)
    #define KEY_S           (15)
    #define KEY_NONE        (0xFF)
    #define KEY_MANY        (0xF0)
#endif

#define KEY_HASH        (KEY_H)
#define KEY_ASTERISK    (KEY_S)

// *** scan_keypad *** return codes ***
#define KEYPAD_0        (0x0002)
#define KEYPAD_1        (0x1000)
#define KEYPAD_2        (0x2000)
#define KEYPAD_3        (0x4000)
#define KEYPAD_4        (0x0100)
#define KEYPAD_5        (0x0200)
#define KEYPAD_6        (0x0400)
#define KEYPAD_7        (0x0010)
#define KEYPAD_8        (0x0020)
#define KEYPAD_9        (0x0040)
#define KEYPAD_A        (0x8000)
#define KEYPAD_B        (0x0800)
#define KEYPAD_C        (0x0080)
#define KEYPAD_D        (0x0008)
#define KEYPAD_H        (0x0004)
#define KEYPAD_S        (0x0001)

#define KEYPAD_NONE     (0x0000)

#define KEYPAD_HASH     (KEYPAD_H)
#define KEYPAD_ASTERISK (KEYPAD_S)

uint16_t scan_keypad    (void);
char     keypressed     (void);
char     getkey         (void);

#endif /* BIOS_KPAD_H_ */
