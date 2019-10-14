#define __MSP430FR2355__
#include <msp430.h> 
#include <msp430gpio.h>

/**
 * main.c
 */

void debounce();

const uint16_t zeroP = 0;
const uint16_t cincoP = 32;
const uint16_t vinteP = 130;
const uint16_t quarentaP = 260;
const uint16_t cemP = 654;


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	disableHighZ;
	
	
	setPin(P6_6, OUTPUT); //steando o led verde
	writePin(P6_6, LOW);
	setPin(P1_0, OUTPUT); //setando o led vermelho
	writePin(P1_0, LOW);

	setPin(P4_1,INPUT_PULL_UP); //Button left
	setPin(P2_3,INPUT_PULL_UP);	//Button Right


	TB0CTL =	TBSSEL__ACLK	|
				MC__UP			|
				TBCLR;
	TB0CCR0 = 654; //frequencia de 50Hz
	TB0CCTL0 = CCIE;

	TB0CCR1 = vinteP;
	TB0CCTL1 = CCIE;// | BIT9 | BITA;


	__enable_interrupt();

	uint16_t val = vinteP;
	while(1) {
	    debounce();
		if(P4IN & BIT1){
			if(readPin(P2_3) == LOW){
			    switch(val) {
			    case zeroP:
			        val = cincoP;
			        break;
			    case cincoP:
			        val = vinteP;
			        break;
			    case vinteP:
			        val = quarentaP;
			        break;
			    case quarentaP:
			        val = cemP;
			        break;
			    case cemP:
			        val = cemP;
			        break;
			    default:
			        break;
			    }
			    TB0CCR1 = val;
			}
			debounce();

		} else{
		            if(readPin(P2_3) == LOW){
		                switch(val) {
		                case zeroP:
		                    val = zeroP;
		                    break;
		                case cincoP:
		                    val = zeroP;
		                    break;
		                case vinteP:
		                    val = cincoP;
		                    break;
		                case quarentaP:
		                    val = vinteP;
		                    break;
		                case cemP:
		                    val = quarentaP;
		                    break;
		                default:
		                    break;
		                }
		                TB0CCR1 = val;
		            }
		            debounce();

		        }
		//TB0CCR1 = val;
	}




	return 0;
}

void debounce() {
    volatile int i = 0;
	for(i = 0;i < 100000; i++);
	return;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_CCRO_ISR() {
	writePin(P6_6, HIGH);
	
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCR1_ISR() {
	switch (TB0IV)
	{
	case 0x2:
		writePin(P6_6,LOW);
		break;
	
	default:
		break;
	}
}
