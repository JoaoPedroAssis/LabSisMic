#include <msp430.h>
#include "msp430gpio.h"

void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    // Led Vermelha
    setPin(P1_0, OUTPUT);

    // Configurando botão
    setPin(P4_1, INPUT_PULL_UP);
    setPin(P2_3, INPUT_PULL_UP);

    // Time para led vermelha
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 655; //50hz
    TB0CCTL0 = CCIE;

    TB0CCR1 = 655 * 0.5;
    TB0CCTL1 = CCIE;

    __enable_interrupt();

    while(1){
        while(readPin(P4_1) == UNPRESSED && readPin(P2_3) == UNPRESSED);
        debounce(10000);
        if (readPin(P4_1) == PRESSED) {
            TB0CCR1 -= 0.1 * TB0CCR0;
        } else if (readPin(P2_3) == PRESSED){
            TB0CCR1 += 0.1 * TB0CCR0;
        }

        while(readPin(P4_1) == PRESSED || readPin(P2_3) == PRESSED);
        debounce(10000);
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
