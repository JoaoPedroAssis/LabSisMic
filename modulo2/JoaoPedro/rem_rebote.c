#include <msp430.h>

#define isPressed(P,B) (P & B)

/**
 * main.c
 */
void rebote(int count) {
    volatile int i;
    i = count;
    while(i--);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Inicializando o LED vermelho
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);

    // Inicilizando a chave S1
    P4DIR &= ~(BIT1);
    P4REN |= (BIT1);
    P4OUT |= (BIT1);

    while(1) {
        while(isPressed(P4IN, BIT1));         // enquanto estiver solto n faz nada
        rebote(1000);
        P1OUT ^= BIT0;                        // Quando apertar troca
        while(!isPressed(P4IN, BIT1));        // Espera desapertar
        rebote(1000);
    }
    return 0;
}
