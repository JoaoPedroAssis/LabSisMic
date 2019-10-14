#include <msp430.h>
#include "msp430gpio.h"

int cont = 1; // Contador inciado com um 1 para iniciar desligado

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    setPin(P6_6, OUTPUT); // Led Verde
    setPin(P1_0, OUTPUT); // Led Vermelha

    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 16383; //1hz
    TB0CCTL0 = CCIE;

    __enable_interrupt();

    while(1){
        // Não faz nada
    }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{
    cont ++;
    switch(cont) {
        case 1:
            writePin(P6_6, HIGH); // LIGA VERDE
            writePin(P1_0, HIGH); // Liga VERMELHO
            break;
        case 2:
            writePin(P6_6, LOW); // DESLIGA VERDE
            writePin(P1_0, LOW); // DESLIGA VERMELHO
            break;
        case 3:
            writePin(P6_6, HIGH); // LIGA VERDE
            writePin(P1_0, LOW); // DESLIGA VERMELHO
            break;
        case 4:
            writePin(P6_6, LOW); // DESLIGA VERDE
            writePin(P1_0, HIGH); // Liga VERMELHO
            cont = 0;
            break;
    }

}
