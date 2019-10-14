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
	TB0CCR0 = 16383;
	TB0CTL = TBSSEL__ACLK | MC__UP;

    setPin(P6_6, OUTPUT);            // Led verde

    writePin(P6_6, LOW);
    writePin(P1_0, HIGH);

	while(1) {
	    while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
	    TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
	    togglePin(P6_6);

    }



	return 0;
}
