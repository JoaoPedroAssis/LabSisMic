#include "msp430helper.h"

void initLED(volatile unsigned char* dir, volatile unsigned char* out, unsigned int bit, unsigned int on) {
    *dir |= bit;
    if (on) {
        *out|= bit;
    } else {
        *out &= ~(bit);
    }
}

void initButton(volatile unsigned char* dir, volatile unsigned char* ren,
                volatile unsigned char* out, unsigned int bit) {
    *dir &= ~(bit);
    *ren |= bit;
    *out |= bit;
}