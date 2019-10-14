#include <msp430.h>
#include "msp430gpio.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    setPin(P1_0, OUTPUT); // Led Vermelha

    // Time para led vermelha
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 655; //50hz
    TB0CCTL0 = CCIE;

    TB0CCR1 = 655 * 0.5;
    TB0CCTL1 = CCIE;

    __enable_interrupt();

    while(1){
        // Não faz nada
    }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCRO_ISR() {
    writePin(P1_0, HIGH);
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR() {
    switch (TB0IV)
    {
    case 0x2: // Canal 1
        writePin(P1_0,LOW);
        break;
    default:
        break;
    }
}
