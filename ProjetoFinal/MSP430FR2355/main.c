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

    lcdPrint("Jogador");
    lcdWriteByte(0xC0, 0);
    lcdPrint("Maquina");

    int var;
    int flag = 2;
    int isMultiplayer = 0;
    while(1) {
        wait(200);
        // Botão pressionado
        if (!(P1IN & BIT6))
            break;

        var = switch_setinha();

        if(var == 1 && flag != 1){
            flag = 1;
            lcdClear();
            lcdPrint("Jogador  <-");
            lcdWriteByte(0xC0, 0);
            lcdPrint("Maquina");
            isMultiplayer = 1;
        }
        else if(var == 0 && flag != 0){
            flag = 0;
            lcdClear();
            lcdPrint("Jogador");
            lcdWriteByte(0xC0, 0);
            lcdPrint("Maquina  <-");
            isMultiplayer = 0;
        }
    }

    // Habilita as interrupções
    // __enable_interrupt();
    print_jogo(a);
    int cont = 0;

    while(!jogoacabou(a))
    {
        // Espera as interrupções
        if(cont % 2 == 0)
            insere_simb(a, 'X');
        else
            if (isMultiplayer)
                insere_simb(a, 'O');
            else
                vezDoPcAleatoria(a, 'O');
        print_jogo(a);
        cont++;
    }
    // Print Vitória
    lcdClear();
    switch (ganhou(a)){
    case -1:
        lcdPrint("X Ganhou");
        break;
    case 1:
        lcdPrint("O Ganhou");
        break;
    case 2:
        lcdPrint("Empate");
        break;
    default:
        lcdPrint("Default");
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
