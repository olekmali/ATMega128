//* External interrupt-based library - header file *
#ifndef BIOS_EXTERNAL_INT_H_
#define BIOS_EXTERNAL_INT_H_
#include <stdint.h>

void    External_Int0_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int0_shutdown  ();

void    External_Int1_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int1_shutdown  ();

void    External_Int4_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int4_shutdown  ();

void    External_Int5_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int5_shutdown  ();

void    External_Int6_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int6_shutdown  ();

void    External_Int7_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int7_shutdown  ();

enum eint_mode {
    EXT_INT_MODE_pin_low = 0,
    EXT_INT_MODE_pin_hi  = 1,
    EXT_INT_MODE_pin_falle = 2,
    EXT_INT_MODE_pin_raise = 3
     };

void    External_Int0_chg_mode  (uint8_t mode);
void    External_Int1_chg_mode  (uint8_t mode);
void    External_Int4_chg_mode  (uint8_t mode);
void    External_Int5_chg_mode  (uint8_t mode);
void    External_Int6_chg_mode  (uint8_t mode);
void    External_Int7_chg_mode  (uint8_t mode);

// PD0 is ExtInt0
// PD1 is ExtInt1
// Warning: ExtInt2 and ExInt3 interfere with UART1 functionality - use only one or the other
// PE4 is ExtInt4
// PE5 is ExtInt5
// PE6 is ExtInt6
// PE7 is ExtInt7


#endif /* BIOS_EXTERNAL_INT_H_ */
