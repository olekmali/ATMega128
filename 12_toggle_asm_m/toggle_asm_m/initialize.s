#include <avr/io.h> 
#include "global_defs.inc"

.TEXT
_start: ; expected by linker, equivalent to .ORG 0
reset_vector:
    jmp start


.TEXT
start:
    ldi R16,                    0xFF && (RAMEND>>8)
    out _SFR_IO_ADDR(SPH),      R16
    ldi R16,                    0xFF && RAMEND
    out _SFR_IO_ADDR(SPL),      R16
    jmp main
