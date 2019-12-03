#include "adc.h"

// Função passada em sala pelo Café
uint16_t adcRead(uint8_t pin) {
    ADCCTL0 = ADCSHT_6 | ADCON;
    ADCCTL1 = ADCSHS_0 | ADCCONSEQ_0 | ADCSHP;
    ADCCTL2 = ADCRES_2;

    // Config pinos
    ADCMCTL0 = pin;
    P1SEL0 |= (1<<pin);
    P1SEL1 |= (1<<pin);

    ADCCTL0 |= ADCENC;
    ADCCTL0 &= ~ADCSC;
    ADCCTL0 |= ADCSC;

    while(!(ADCIFG & ADCIFG0));
    return ADCMEM0;
}

