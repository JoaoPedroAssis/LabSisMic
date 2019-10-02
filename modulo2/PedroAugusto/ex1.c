#include <msp430.h> 

int main(void)
{
    // Configurando pinos
    P4DIR &= ~(BIT1);
    P4REN |= BIT1;
    P4OUT |= BIT1;

    // Configurando pinos da LED
    P6DIR |= BIT6;
    P6OUT &= ~(BIT6);

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	while (1) {
	    if (P4IN & BIT1) {
	        P6OUT &= ~(BIT6);
	    } else {
	        P6OUT |= BIT6;
	    }
	}
	return 0;
}
