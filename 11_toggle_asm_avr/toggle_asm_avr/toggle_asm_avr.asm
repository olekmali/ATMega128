// * blink2 program *
.INCLUDE "m128def.inc"


.EQU LED_alive = 7
.EQU LED0 = 0
.EQU LED1 = 1
.EQU KEY0 = 0
.EQU KEY1 = 1


.CSEG
.ORG $0000
reset_vector:
    jmp start


.DSEG
.ORG $0100

; what was sent to the LED output port previously, or what is about to be sent there
led_status:
    .BYTE 1
; previous status of push buttons saved so that any difference can be detected
key_last:
    .BYTE 1

.CSEG
start:
    ldi R16,   HIGH(RAMEND);
    out sph,    R16
    ldi R16,   LOW(RAMEND);
    out spl,    R16

;   Initialize global variables in RAM
    eor     R16,        R16     ; you may want to start with a different value in case of HW, use ldi then
    sts     led_status, R16
;   eor     R16,        R16     ; no need to reset to 0 again
    sts     key_last,   R16

main:

main_initialize:
;   PUD: Pull-up enable (already done)
;   in      R16,       SFIOR
;   andi    R16,       ~(0b00000100)
;   out     SFIOR,      R16;

;   Reset portD contents to prevent LED flickering during this setup
;   This actually is optional on power up as 0 is the default initial state
    eor     R16,        R16
    out     PortD,      R16
;   Using PortD pins number LED_alive and LED0 as outputs
    ldi     R16,       (1<<LED_alive | 1<<LED0)
    out     DDRD,       R16

;   Reassuring that PortB pin KEY0 is set as an input with pull up resistor active
    eor     R16,        R16
    out     DDRB,       R16    ; set the entire port as IN (already done upon reset)
    ldi     R16,        1<<KEY0
    out     PortB,      R16    ; activate pull up resistors on used input

main_superloop:
    ldi     R17,        1<<LED_alive
    lds     R16,        led_status
    eor     R16,        R17
    sts     led_status, R16

    in      R16,        PinB
    lds     R18,        key_last
    eor     R18,        R16
    sts     key_last,   R16

check_k0:
    mov     R16,        R18
    andi    R16,        1<<KEY0
    breq    skip_k0
    lds     R16,        key_last
    andi    R16,        1<<KEY0
    breq    skip_k0
toggle_led0:
    ldi     R17,        1<<LED0
    lds     R16,        led_status
    eor     R16,        R17
    sts     led_status, R16
skip_k0:


    lds     R16,        led_status
    out     PortD,      R16

    ldi     R17,        100     ; ask for 100ms delay
    mov     R2,         R17
; note: perhaps it would be better to design the function to receive its parameter in R16 or above
; as that would allow to set constant parameter with just one instruction instead of two?
    rcall   delay_in_ms


    rjmp    main_superloop


    ; this function
    ;   - receives a short int variable in registers [r2] and returns 0 in [r2]
    ;   - causes time delay of that many milliseconds by wasting time and energy
    ;
.EQU OneMS = 16*1000/5

delay_in_ms:
    push    R16
    in      R16,   sreg
    push    R16

    .def    d0  = R17
    .def    d1  = R18
    .def    d2  = R19

    push    d0
    push    d1
    push    d2

delay_count:
    ldi     d2,     byte3 (OneMS)
    ldi     d1,     high  (OneMS)
    ldi     d0,     low   (OneMS)

delay_loop:
    subi    d0,     1
    sbci    d1,     0
    sbci    d2,     0
    brcc    delay_loop

    dec     R2
    brne    delay_count

    pop     d2
    pop     d1
    pop     d0

    pop     R16
    out     sreg,   R16
    pop     R16
    ret
