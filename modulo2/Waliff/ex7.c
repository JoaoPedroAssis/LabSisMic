#include <msp430.h> 
#include "msp430gpio.h"


/**
 * main.c
 */

unsigned char rand() {
    static unsigned char num = 5;
    num = (num * 17) % 7;
    return num;
}

void debounce (volatile int i) {
    while(i--);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	
    setPin(P1_0, OUTPUT);            // Led vermelho
    setPin(P6_6, OUTPUT);            // Led verde
    setPin(P4_1, INPUT_PULL_UP);     // set button 1
    setPin(P2_3, INPUT_PULL_UP);     // set button 2

    int contador = 0;
    int num = 2;

    while(1) {
        while(readPin(P2_3) == UNPRESSED || readPin(P4_1) == UNPRESSED);
        debounce(1000);

        if(readPin(P2_3) == PRESSED){
            if (num == contador){
                writePin(P6_6, HIGH);
                writePin(P1_0, LOW);
                num = rand();
            }
            else {
                writePin(P6_6, LOW);
                writePin(P1_0, HIGH);
            }
            contador = 0;
        }
        if(readPin(P4_1) == PRESSED){
            contador += 1;
        }
        while(readPin(P2_3) == PRESSED || readPin(P4_1) == PRESSED);
    }



	return 0;
}
