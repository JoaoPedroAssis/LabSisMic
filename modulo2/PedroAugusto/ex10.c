#include <msp430.h>
#include "msp430gpio.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    setPin(P6_6, OUTPUT); // Led Verde


    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 16383;
    TB0CCTL0 = CCIE;

    __enable_interrupt();

    while(1){
        // Não faz nada
    }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{
     togglePin(P6_6);
}
