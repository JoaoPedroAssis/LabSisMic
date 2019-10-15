#include <msp430.h>
#include "msp430gpio.h"

void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}

#define seg1 8192
#define seg3 24576

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    // Leds Vermelha e Verde
    setPin(P1_0, OUTPUT);
    setPin(P6_6, OUTPUT);

    // Configurando botão
    setPin(P4_1, INPUT_PULL_UP);
    setPin(P2_3, INPUT_PULL_UP);

    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR | ID__4;
    TB0CCR0 = 32767; // 4s
    TB0CCTL0 = CAP | CM__RISING;

    __enable_interrupt();

    while(1){
        TB0CCTL0 = CCIS_2;
        while(readPin(P4_1) == UNPRESSED || readPin(P2_3) == UNPRESSED);
        debounce(10000);
        if (readPin(P4_1) == PRESSED) {
            TB0CTL |= TBCLR;
            TB0CCTL0 &= ~(CCIFG);
        } else if (readPin(P2_3) == PRESSED){
            // Se estourou o contador passou de 3 segundos
            TB0CCTL0 = CCIS_3;
            if (TB0CCTL0 & CCIFG){
                setPin(P1_0, HIGH);
                setPin(P6_6, HIGH);
            // Menos de 1 segundos
            } else if (TB0CCR0 < seg1) {
                setPin(P1_0, HIGH);
                setPin(P6_6, LOW);
            // Menos de 3 segundos
            } else if (TB0CCR0 < seg3) {
                setPin(P1_0, LOW);
                setPin(P6_6, HIGH);
            // Mais de 3 segundos
            } else {
                setPin(P1_0, HIGH);
                setPin(P6_6, HIGH);
            }
        }

        while(readPin(P4_1) == PRESSED || readPin(P2_3) == PRESSED);
        debounce(10000);
    }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCRO_ISR() {
    writePin(P1_0, HIGH);
}
