#include <msp430.h>
#include <stdint.h>
#define isPressed(P,B) (P & B)

void rebote(volatile int count) {
    while(count--);
}

void initLED(volatile unsigned char* dir, volatile unsigned char* out,
             unsigned int bit, unsigned int on) {
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
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    initLED(&P1DIR, &P1OUT, BIT0, 1);
    initButton(&P4DIR, &P4REN, &P4OUT, BIT1);
    initButton(&P2DIR, &P2REN, &P2OUT, BIT3);

    while(1) {
        while(isPressed(P4IN, BIT1) && isPressed(P2IN, BIT3));
        rebote(1000);
        P1OUT ^= BIT0;
        while(!(isPressed(P4IN, BIT1) && isPressed(P2IN, BIT3)));
        rebote(1000);
    }

    return 0;
}
