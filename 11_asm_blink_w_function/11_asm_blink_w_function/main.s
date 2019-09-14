// * blink1 program *
#include <avr/io.h>

.GLOBAL main

.TEXT
_start:
    jmp     start
; .. insert interrupt vector here, 32bits per entry, up to 34 entries

; this is the loader program that initializes the stack etc.
.TEXT
start:
    ldi     R16,                    0xFF && (RAMEND>>8)
    out     _SFR_IO_ADDR(SPH),      R16
    ldi     R16,                    0xFF && RAMEND
    out     _SFR_IO_ADDR(SPL),      R16

; main program function starts here
main:

main_initialize:
    cbi     _SFR_IO_ADDR(PORTA),  7
;  Using PORTA, Pin7 as output
;                     76543210
   ldi      R16,    0b10000000  ; or $80 or 128 or 1 << 7
   out      _SFR_IO_ADDR(DDRA),   R16

main_superloop:
    ; LED on
    sbi     _SFR_IO_ADDR(PORTA),  7
    ; and wait
    ldi     R21,    0
    ldi     R20,    100
    rcall   delay_in_ms

    ; LED off
    cbi     _SFR_IO_ADDR(PORTA),  7
    ; and wait
    ldi     r21,    0
    ldi     r20,    100
    rcall   delay_in_ms

    ; and do ti forever
    rjmp main_superloop


    ; this function
    ;   - T.B.A. receives a short int variable in registers [r21,r20]
    ;   - causes time delay of that many milliseconds by wasting time and energy
    ;
delay_in_ms:
    push    R16
    in      R16, _SFR_IO_ADDR(SREG)
    push    R16

    .equ    OneMS,  16000000 / 5 / 1000 ; 16000 for quartz FRQ, /1000 for 1 ms, /5 for the duration of the loop 

    push    R17
    push    R18
    push    R19

; TO DO: add the loop to run the delay [r21,r20] times
ms_counter_loop:

    ldi     R19,         0xFF & ( OneMS >> 16 )
    ; note -- we only need two registers but that would speed up the loop and mabye we would have to go back to three registers to keep the count
    ldi     R18,         hi8  (OneMS)      ; or 0xFF & ( OneMS >> 8 )
    ldi     R17,         lo8  (OneMS)      ; or 0xFF &   OneMS

delay_loop:
    subi    R17,         1
    sbci    R18,         0
    sbci    R19,         0
    brcc    delay_loop

    subi    R20,        1
    sbci    R21,        0
    brcc    ms_counter_loop

    pop     R19
    pop     R18
    pop     R17

    pop     R16
    out     _SFR_IO_ADDR(SREG),   R16
    pop     R16
    ret
