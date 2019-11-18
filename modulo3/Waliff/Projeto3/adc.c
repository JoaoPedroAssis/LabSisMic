#include "adc.h"

static uint8_t dataFinish = 0;
static uint8_t dataPtr[16];
void adcConfig(uint8_t pin)
{
    ADCCTL0 = ADCON;
    ADCCTL1 = ADCSHS_2 | ADCCONSEQ_2;
    //ADCCTL2 = ADCRES1;
    ADCMCTL0 = pin;
    P1SEL0 |= (1<<pin);
    P1SEL1 |= (1<<pin);
    ADCCTL0 |= ADCENC;
    ADCIE = ADCIE0;
    TB1CTL = MC__UP |
             TBCLR  |
             TBSSEL__SMCLK;
    TB1CCR0 = 50000-1;
    TB1CCR1 = 6900 -1;
    TB1CCTL1 = OUTMOD_7;

}

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

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR()
{
    static uint8_t dataCount = 0;
    *(dataPtr+dataCount) = ADCMEM0;
    if(dataCount == 15)
    {
       // adcFinish = 1;
    }
}
