#include <msp430.h> 
#include "msp430gpio.h"

void rebote(volatile int count) {
    while(count--);
}
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    disableHighZ;

    setPin(P4_1, INPUT_PULL_UP); // S1
    setPin(P2_3, INPUT_PULL_UP); // S2
    setPin(P1_0, OUTPUT); // led vermelho
    setPin(P6_6, OUTPUT); // led verde

    writePin(P1_0, HIGH);
    writePin(P6_6, HIGH);
    while(1) {
        while(readPin(P4_1) == UNPRESSED && readPin(P2_3) == UNPRESSED);
        rebote(1000);
        if (readPin(P4_1) == PRESSED && readPin(P2_3) == PRESSED) {
            writePin(P1_0, LOW);
            writePin(P6_6, LOW);
            while(readPin(P4_1) == PRESSED && readPin(P2_3) == PRESSED);
            rebote(1000);
        } else if (readPin(P4_1) == PRESSED) {
            togglePin(P1_0);
            togglePin(P6_6);
            while(readPin(P4_1) == PRESSED);
            rebote(1000);
        } else if (readPin(P2_3) == PRESSED) {
            togglePin(P6_6);
            while(readPin(P2_3) == PRESSED);
            rebote(1000);
        }
    }
}
