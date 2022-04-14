// * In, shift, and out, repeat that forever *
#include <avr/io.h>

.GLOBAL main
; this is our main program
.TEXT
main:
    ; set pin 0 of port A to a push-pull output
    ; other pins kept as inputs without pull-up resistors
    eor     R16,                        R16
    out     _SFR_IO_ADDR(PORTA),        R16
    ldi     R16,                        0b00000001
    out     _SFR_IO_ADDR(DDRA),         R16

    ; set pin 4 of port E to input with a pull-up resistor
    ; other pins kept as inputs without pull-up resistors
    ldi     R16,                        0b00010000
    out     _SFR_IO_ADDR(PORTE),        R16
    ldi     R16,                        0b00000000
    out     _SFR_IO_ADDR(DDRE),         R16

    ; set up a constant to "negate" the push-button key status using ExclusiveOR
    ; push-button pushed yields input as 0
    ; push-button not pushed yields input as 1 due to the pull-up resistor 
    ldi     R17,                        0b00010000

    ; main program forever loop
loop:
    in      R16,                        _SFR_IO_ADDR(PINE)
    andi    R16,                        0b00010000
    eor     R16,                        R17
    lsr     R16
    lsr     R16
    lsr     R16
    lsr     R16
    out     _SFR_IO_ADDR(PORTA),        R16
    rjmp loop
