#include <msp430.h>
#include "msp430gpio.h"
#define TimerBConfiguration(clock, mode) (TBCLR | (TBSSEL__##clock) | (MC_##mode))

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

   // N = Tempo/Tclk -> N=16.384 -> com ACLK
    setPin(P6_6, OUTPUT); // Verde

    TB0CCR0 = 16383;
    TB0CTL = TBSSEL__ACLK | MC__UP;


    while(1) {
        while(!(TB0CCTL0 & CCIFG));
        TB0CCTL0 &= ~CCIFG;
        togglePin(P6_6);
    }
}

