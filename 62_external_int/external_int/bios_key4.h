//* bios_key4.h *
#ifndef BIOS_KEY4_H_
#define BIOS_KEY4_H_
#include <stdint.h>

#define B_K7 (0x08)
#define B_K6 (0x04)
#define B_K5 (0x02)
#define B_K4 (0x01)

void    key4_init       (void);
void    key4_shutdown   (void);
uint8_t key4_get        (void);

#endif /* BIOS_KEY4_H_ */