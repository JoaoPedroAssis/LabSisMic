#include <msp430.h>
#include "msp430gpio.h"
#define is ==
#define TimerBConfiguration(clock, mode) (TBCLR | (TBSSEL__##clock) | (MC_##mode))
/**
 * main.c
 */
int coletaPeriodo();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	disableHighZ;

	// Configurando LEDs
	setPin(P1_0, OUTPUT); // Verde
	setPin(P6_6, OUTPUT); // Vermelho

	// Configurando inputs
	setPin(P1_2, INPUT);

	// Configurando outputs
	setPin(P6_0, OUTPUT);
	setPin(P6_1, OUTPUT);
	setPin(P6_2, OUTPUT);
	setPin(P6_3, OUTPUT);
	
	// Configurando frequencia 20%
	writePin(P6_0, HIGH);
	writePin(P6_1, LOW);

	// Habilitando timer
	TB0CTL = TBSSEL__SMCLK | MC__CONTINOUS;

	int vermelho, verde, azul;
	while(1) {
	    // Checando Vermelho
	    writePin(P6_2, LOW);
	    writePin(P6_3, LOW);
	    vermelho = coletaPeriodo();

	    // Checando Verde
	    writePin(P6_2, HIGH);
	    writePin(P6_3, HIGH);
	    verde = coletaPeriodo();

	    // Checando Azul
	    writePin(P6_2, LOW);
	    writePin(P6_3, HIGH);
	    azul = coletaPeriodo();

	    if (vermelho < azul && vermelho < verde) {
	        writePin(P6_6, HIGH);
	        writePin(P1_0, LOW);
	    } else if (azul < vermelho && azul < verde) {
	        writePin(P6_6, HIGH);
	        writePin(P1_0, HIGH);
	    } else {
	        writePin(P6_6, LOW);
	        writePin(P1_0, HIGH);
	    }
	}
}

int coletaPeriodo() {
    unsigned int xi = 0, xf = 0;
    int diff;
    unsigned int i, n;
    n = 20;
    int vecDiff[n];

    for (i = 0; i < n; i++) {
        while(readPin(P1_2) == LOW);
        xi = xf;
        xf = TB0R;
        diff = xf - xi;
        if (diff < 0) {
            diff += 0xFFFF + 1;
        }
        vecDiff[i] = diff;
        while(readPin(P1_2) == HIGH);
    }

    int media;
    for (i = 1; i < n; i++) {
        media += vecDiff[i];
    }
    return media/n;
}
