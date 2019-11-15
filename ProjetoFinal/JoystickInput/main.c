#include <msp430.h> 
#include "i2c.h"
#include "lcd.h"
#include "adc.h"
#include "jogodavelha.h"


Arv* a;

void main(void)
{
    // Incia a árvore de possibilidades do jogo
    a = cria_vazia();

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Configurando o LED para piscar quando atualizar o LCD
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // Configurando SWITCH
    P1DIR &= ~BIT6;
    P1REN |= BIT6;
    P1OUT |= BIT6;

    // Configurando timer
    //TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    //TB0CCR0 = (0xFFFF/2); // 1seg
    //TB0CCTL0 = CCIE; // habilita interrupções

    // Incializa o i2c e o lcd
    i2cInit(1, 0);
    lcdInit();

    // Habilita as interrupções
    // __enable_interrupt();
    print_jogo(a);
    while(1)
    {
      // Espera as interrupções
        insere_simb(a, 'X');
        print_jogo(a);
    }
}

/*
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{
    static int i = 0;
    static uint16_t x = 0;
    static uint16_t y = 0;

    // Clear
    lcdClear();
    x = 0;
    y = 0;

    // Trocar estado do led (Bit Toogle)
    P1OUT ^= BIT0;

    for (i = 0; i < 8; i++) {
        // Ler 8 vezes de cada canal
        x += adcRead(4);
        y += adcRead(5);
    }

    // Printa no lcd a voltagem em X (Canal 4)
    //joystick_jogada(x, y);
    print_jogo(a);
}
*/
