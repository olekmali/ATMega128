//* bios_led8.h *
#ifndef BIOS_LED8_H_
#define BIOS_LED8_H_
#include <stdint.h>

#define B_L0 (0x01)
#define B_L1 (0x02)
#define B_L2 (0x04)
#define B_L3 (0x08)
#define B_L4 (0x10)
#define B_L5 (0x20)
#define B_L6 (0x40)
#define B_L7 (0x80)

void    led8_init       (void);
void    led8_shutdown   (void);
uint8_t led8_get        (void);
void    led8_set        (uint8_t number);
void    delay           (unsigned short int milliseconds);

#endif /* BIOS_LED8_H_ */