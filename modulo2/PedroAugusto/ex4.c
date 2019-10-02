#include <msp430.h> 
#include "msp430gpio.h"

/**
 * main.c
 */
void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5; // Ativar os pinos (desabilitar modo de alto impedância)

	// Configurando botão
	setPin(P4_1, INPUT_PULL_UP);
	setPin(P2_3, INPUT_PULL_UP);
	// Configurando led vermelha
	setPin(P1_0, OUTPUT);

	while(1){
	    // Borda de subida
	    while(isPressed(P4IN, BIT1) && isPressed(P2IN, BIT3));
	    debounce(1000);
	    if(readPin(P1_0)) {
	        writePin(P1_0, LOW);
	    } else {
	        writePin(P1_0, HIGH);
	    }
	    while(!isPressed(P4IN, BIT1) || !isPressed(P2IN, BIT3));
	    debounce(1000);
	}
}
