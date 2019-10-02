#include <msp430.h> 
#include "msp430gpio.h"

/**
 * main.c
 */
void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}

void alterna_estado_led(char led){
    if(readPin(led)) {
        writePin(led, LOW);
    } else {
        writePin(led, HIGH);
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

	while(1){
	    // Borda de subida
	    while(readPin(P4_1) == UNPRESSED && readPin(P2_3) == UNPRESSED);
	    debounce(1000);
	    // Se os dois forem pressionados
	    if(readPin(P4_1) == PRESSED && readPin(P2_3) == PRESSED){
	        // Apaga os dois ledes
	        writePin(P1_0, LOW);
	        writePin(P6_6, LOW);
	        // Caso aperte S1
	    } else if (readPin(P4_1) == PRESSED) {
	        //Alterna o estado dos dois leds
	        alterna_estado_led(P1_0);
	        alterna_estado_led(P6_6);
	        // Caso aperte S2
	    } else if (readPin(P2_3) == PRESSED){
	        // Alterna o estado somente do ver
	        alterna_estado_led(P6_6);
	    }
	    while(readPin(P4_1) == PRESSED || readPin(P2_3) == PRESSED);
	    debounce(1000);
	}
}
