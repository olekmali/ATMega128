//* bios.h *
#ifndef BIOS_H_
#define BIOS_H_
#include <stdint.h>

void    init_digital_io ();
void    display_byte    (uint8_t number);
void    delay           ( unsigned short int milliseconds );

#endif /* BIOS_H_ */