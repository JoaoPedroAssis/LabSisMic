#include <msp430.h> 
#include "msp430gpio.h"

/**
 * main.c
 */
void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}

void num_to_led(short int num){
    switch (num) {
        case 1:
            writePin(P1_0, LOW);
            writePin(P6_6, HIGH);
            break;
        case 2:
           writePin(P1_0, HIGH);
           writePin(P6_6, LOW);
           break;
        case 3:
           writePin(P1_0, HIGH);
           writePin(P6_6, HIGH);
           break;
        default:
            writePin(P1_0, LOW);
            writePin(P6_6, LOW);
            break;
    }
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
    // Configurando led verde
    setPin(P6_6, OUTPUT);

    short int cont=0;
    num_to_led(cont);
	while(1){
	    while(readPin(P4_1) == UNPRESSED && readPin(P2_3) == UNPRESSED);
	    debounce(1000);
	    // Se os dois forem pressionados
	    if(readPin(P4_1) == PRESSED && readPin(P2_3) == PRESSED){
	        // Do nothing
	    } else if (readPin(P4_1) == PRESSED) {// Caso aperte S2 Decrementa
	        if (cont <= 0) {
	            // Overflow
	            cont = 3;
	        } else {
	            cont-- ;
	        }
	    } else if (readPin(P2_3) == PRESSED){ // Caso aperte S2 Incrementa
	        if (cont >= 3) {
	            // Overflow
	            cont = 0;
	        } else {
	            cont++ ;
	        }
	    }
	    num_to_led(cont);

	    while(readPin(P4_1) == PRESSED || readPin(P2_3) == PRESSED);
	    debounce(1000);
	}
}
