#include <avr/io.h> 
#include "global_defs.inc"

    ; this function
    ;   - T.B.A. receives a unit8_t  variable in register [r2]
    ;   - causes time delay of that many miliseconds by wasting time and energy
    ;
.GLOBAL delay_in_ms

.EQU OneMS, (16*1000/5) ; 3072


.TEXT
delay_in_ms:
    push    R16
    in      R16,        _SFR_IO_ADDR(SREG)
    push    R16

    push    R17
    push    R18
    push    R19

count_ms:
    ldi     R19,        ( 0xFF & ( OneMS>>16 ) ) ; it is 0 but we need 3byte counter for timing purpose
    ldi     R18,        ( 0xFF & ( OneMS>> 8 ) )
    ldi     R17,        ( 0xFF &   OneMS       )

delay_1ms:
    subi    R17,        1
    sbci    R18,        0
    sbci    R19,        0
    brcc    delay_1ms

    dec     R3
    brne    count_ms

    pop     R19
    pop     R18
    pop     R17

    pop     R16
    out     _SFR_IO_ADDR(SREG),     R16
    pop     R16
    ret
