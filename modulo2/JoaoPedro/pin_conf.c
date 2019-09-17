#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	P4DIR &= ~(BIT1);
	P4REN |= (BIT1);
	P4OUT |= (BIT1);

	P6DIR |= BIT6;
	P6OUT &= ~(BIT6);

	while(1) {
	    if(P4IN & BIT1) {
	        P6OUT &= ~(BIT6);
	    } else {
	        P6OUT |= BIT6;
	    }
	}
	
	return 0;
}
