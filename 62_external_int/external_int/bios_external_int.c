//* External interrupt-based library - implementation file *
#include "bios_external_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*External_interrupt0_function)(void) = 0;
static void (*External_interrupt1_function)(void) = 0;
static void (*External_interrupt4_function)(void) = 0;
static void (*External_interrupt5_function)(void) = 0;
static void (*External_interrupt6_function)(void) = 0;
static void (*External_interrupt7_function)(void) = 0;

void External_Int0_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt0_function = handle_fn;
    External_Int0_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT0;
    EIFR  = EIFR  | 1<<INTF0;
}

void External_Int1_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt1_function = handle_fn;
    External_Int1_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT1;
    EIFR  = EIFR  | 1<<INTF1;
}

void External_Int4_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt4_function = handle_fn;
    External_Int4_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT4;
    EIFR  = EIFR  | 1<<INTF4;
}

void External_Int5_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt5_function = handle_fn;
    External_Int5_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT5;
    EIFR  = EIFR  | 1<<INTF5;
}

void External_Int6_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt6_function = handle_fn;
    External_Int6_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT6;
    EIFR  = EIFR  | 1<<INTF6;
}

void External_Int7_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt7_function = handle_fn;
    External_Int7_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT7;
    EIFR  = EIFR  | 1<<INTF7;
}

void External_Int0_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT0);
}

void External_Int1_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT1);
}

void External_Int4_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT4);
}

void External_Int5_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT5);
}

void External_Int6_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT6);
}

void External_Int7_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT7);
}

void External_Int0_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicra_old = EICRA & ~(1<<ISC01|1<<ISC00);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRA = eicra_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRA = eicra_old | 1<<ISC00;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRA = eicra_old | 1<<ISC01;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRA = eicra_old | 1<<ISC01 | 1<<ISC00;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}

void External_Int1_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicra_old = EICRA & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRA = eicra_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRA = eicra_old | 1<<ISC10;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRA = eicra_old | 1<<ISC11;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRA = eicra_old | 1<<ISC11 | 1<<ISC10;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}

void External_Int4_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicrb_old = EICRB & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRB = eicrb_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRB = eicrb_old | 1<<ISC40;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRB = eicrb_old | 1<<ISC41;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRB = eicrb_old | 1<<ISC41 | 1<<ISC40;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}

void External_Int5_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicrb_old = EICRB & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRB = eicrb_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRB = eicrb_old | 1<<ISC50;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRB = eicrb_old | 1<<ISC51;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRB = eicrb_old | 1<<ISC51 | 1<<ISC50;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}

void External_Int6_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicrb_old = EICRB & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRB = eicrb_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRB = eicrb_old | 1<<ISC60;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRB = eicrb_old | 1<<ISC61;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRB = eicrb_old | 1<<ISC61 | 1<<ISC60;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}

void External_Int7_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;           // preserve current interrupt status
    cli();
    uint8_t eicrb_old = EICRB & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRB = eicrb_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRB = eicrb_old | 1<<ISC70;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRB = eicrb_old | 1<<ISC71;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRB = eicrb_old | 1<<ISC71 | 1<<ISC70;
        break;
    } // switch
    SREG = sreg_save;                   // restore interrupts
}


ISR(INT0_vect)
{
    External_interrupt0_function();
}

ISR(INT1_vect)
{
    External_interrupt1_function();
}

ISR(INT4_vect)
{
    External_interrupt4_function();
}

ISR(INT5_vect)
{
    External_interrupt5_function();
}

ISR(INT6_vect)
{
    External_interrupt6_function();
}

ISR(INT7_vect)
{
    External_interrupt7_function();
}
