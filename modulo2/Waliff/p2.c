#include <msp430.h> 
#include "msp430gpio.h"

void debounce (long unsigned int i) {
    volatile int j;
    for(j = 0; j < i; j++);
}

/**
 * main.c
 */
int main(void)
{
    int  contador = 0;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    TB0CCR0 = 3276; // 100ms
    TB0CTL = TBSSEL__ACLK | MC__UP;

    setPin(P2_3, INPUT_PULL_UP);     // Botão S2
    setPin(P4_1, INPUT_PULL_UP);     // Botão S1
    setPin(P1_0, OUTPUT);            // Led vermelho
    writePin(P1_0, LOW);             // Desliga o vermelho
    setPin(P6_6, OUTPUT);            // Led vermelho
    writePin(P6_6, LOW);             // Desliga o verde

    while(P2IN&BIT3 || P4IN&BIT1);
    //while(readPin(P2_3) == UNPRESSED || readPin(P4_1) == UNPRESSED);

    while(1){
        debounce(10000);

        //writePin(P6_6, LOW);
        // Botões foram pressionados
        TB0CCR0 = 3276; // 100ms
        while(contador < 3){
            togglePin(P1_0);             // Liga o led por 100ms
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            togglePin(P1_0);
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            // Ligou e desligou por 100 ms
            contador++;
        }
        // Ligou e desligou por 100 ms 3 vezes
        contador = 0;

        // Esperando 300 ms
        TB0CCR0 = 9829; // 300ms
        while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
        TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG


        while(contador < 3){
            TB0CCR0 = 9829; // 300ms
            togglePin(P1_0);             // Liga o led por 100ms
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            TB0CCR0 = 3276; // 100ms
            togglePin(P1_0);
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            // Ligou e desligou por 100 ms
            contador++;
        }
        contador = 0;

        // Esperando 300 ms
        TB0CCR0 = 9829; // 300ms
        while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
        TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG

        TB0CCR0 = 3276; // 100ms
        while(contador < 3){
            togglePin(P1_0);             // Liga o led por 100ms
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            togglePin(P1_0);
            while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
            TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG
            // Ligou e desligou por 100 ms
            contador++;
        }
        // Ligou e desligou por 100 ms 3 vezes
        contador = 0;

        // Esperando 700 ms
        //writePin(P6_6, HIGH);
        TB0CCR0 = 22936; // 700ms
        while(!(TB0CCTL0 & CCIFG)); // Aguarda o final da contagem
        TB0CCTL0 &= ~CCIFG; // Zera a flag CCIFG


        debounce(10000);
    }

    return 0;
}
