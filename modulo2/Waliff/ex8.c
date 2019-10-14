#include <msp430.h> 
#include "msp430gpio.h"


/**
 * main.c
 */

void debounce (long unsigned int i) {
    volatile int j;
    for(j = 0; j < i; j++);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	
    setPin(P1_0, OUTPUT);            // Led vermelho
    setPin(P6_6, OUTPUT);            // Led verde

    while(1) {
        writePin(P1_0, LOW);
        writePin(P6_6, LOW);
        debounce(500000);
        writePin(P6_6, HIGH);
        writePin(P1_0, HIGH);
        debounce(500000);


    }



	return 0;
}
