#include <msp430.h>
#include <stdint.h>
#include "msp430gpio.h"

void rebote(volatile int count) {
    while(count--);
}
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    setPin(P1_0, OUTPUT);
    setPin(P4_1, INPUT_PULL_UP);
    setPin(P2_3, INPUT_PULL_UP);

    while(1) {
        while(isPressed(P4IN, BIT1) && isPressed(P2IN, BIT3));
        rebote(1000);
        P1OUT ^= BIT0;
        while(!(isPressed(P4IN, BIT1) && isPressed(P2IN, BIT3)));
        rebote(1000);
    }

    return 0;
}
