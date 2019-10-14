#include <msp430.h>
#include "msp430gpio.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    setPin(P6_6, OUTPUT); // Led Verde
    setPin(P1_0, OUTPUT); // Led Vermelha

    // Timer para led vermelha
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 16383; //2hz
    TB0CCTL0 = CCIE;

    // Timer para led verde
    TB1CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB1CCR0 = 6554; //5hz
    TB1CCTL0 = CCIE;

    __enable_interrupt();

    while(1){
        // Não faz nada
    }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{
    togglePin(P1_0);
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void TB1_ISR()
{
    togglePin(P6_6);
}

