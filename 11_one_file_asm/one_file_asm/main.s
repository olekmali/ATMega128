// * In, shift, and out, repeat that forever *
#include <avr/io.h>

.GLOBAL main

.TEXT
_start: ; expected by linker, equivalent to the "old style" .ORG 0
    jmp main

; setting up stack pointer that is needed for function calls
;.TEXT
;start:
;    ldi     R16,                    0xFF && (RAMEND>>8)
;    out     _SFR_IO_ADDR(SPH),      R16
;    ldi     R16,                    0xFF && RAMEND
;    out     _SFR_IO_ADDR(SPL),      R16
;    jmp     main

; this is our main program
.TEXT
main:
    ; set pin 0 of port A to a push-pull output
    ; other pins kept as inputs without pullup resistors
    eor     R16,                        R16
    out     _SFR_IO_ADDR(PORTA),        R16
    ldi     R16,                        0b00000001
    out     _SFR_IO_ADDR(DDRA),         R16

    ; set pin 4 of port E to input with a pullup resistor
    ; other pins kept as inputs without pullup resistors
    ldi     R16,                        0b00010000
    out     _SFR_IO_ADDR(PORTE),        R16
    ldi     R16,                        0b00000000
    out     _SFR_IO_ADDR(DDRE),         R16

    ; set up a constant to "negate" the pushbutton key status using ExclusiveOR
    ; pushbutton pushed yileds input as 0
    ; pushbutton not pusehd yileds input as 1 due to the pullup resitor 
    ldi     R17,                        0b00010000

    ; main program forever loop
loop:
    in      R16,                        _SFR_IO_ADDR(PINE)
    andi    R16,                        0b00010000
    eor     R26,                        R17
    lsr     R16
    lsr     R16
    lsr     R16
    lsr     R16
    out     _SFR_IO_ADDR(PORTA),        R16
    rjmp loop
