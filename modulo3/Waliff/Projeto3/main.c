#include <msp430.h> 
#include <stdint.h>
#include "i2c.h"
#include "lcd.h"
#include "adc.h"


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Configurando o LED
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // Configurando pinos
    P4DIR &= ~(BIT1);
    P4REN |= BIT1;
    P4OUT |= BIT1;

    // Configurando timer
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = (0xFFFF/2);
    TB0CCTL0 = CCIE;

    // Config
    i2cInit();
    lcdInit();

    __enable_interrupt();

    while(1){
        // Waliff Cordeiro - 17/0115810
    }

}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{
    static int i = 8;
    static uint16_t x = 0;
    static uint16_t y = 0;
    static char string[6];
    static char stringHexa[5];
    uint16_t xMSB, xLSB, yMSB, yLSB;

    // LED Toggle
    P1OUT ^= BIT0;

    // Clear
    lcdClear();
    x = 0;
    y = 0;

    while(i--){
        x += adcRead(5);
        y += adcRead(4);
    }
    i = 8;

    // Linha 1 - X
    vtoh((uint16_t) (x/8), stringHexa);
    x = (uint16_t)(((x/8)*3.3/4096) * 1000);

    // Separando parte inteira e fracionária
    xMSB = x/1000;
    xLSB = x%1000;

    // Formatando print - Eixo X
    lcdPrint("X: ");
    vtos((xMSB), string);
    lcdPrint(string);
    lcdPrint(".");
    vtos((xLSB), string);

    // Mantendo sempre 3 casas decimais
    if(xLSB >= 100){
        lcdPrint(string);
    }
    else if(xLSB >= 10){
        lcdPrint(string);
        lcdPrint("0");
    }
    else{
        lcdPrint(string);
        lcdPrint("0");
        lcdPrint("0");
    }
    lcdPrint("V");

    // Print Hexa X
    lcdPrint("   ");
    lcdPrint(stringHexa);

    // Pular para próxima linha
    lcdWriteByte(0xC0, 0);

    // Linha 2 - Y
    vtoh((uint16_t) (y/8), stringHexa);

    // 3.3V e 4096 (máximo em 12 bits)
    y = (uint16_t)(((y/8)*3.3/4096) * 1000);

    // Separando parte inteira e fracionária
    yMSB = y/1000;
    yLSB = y%1000;

    // Formatando print - Eixo Y
    lcdPrint("Y: ");
    vtos((yMSB), string);
    lcdPrint(string);
    lcdPrint(".");
    vtos((yLSB), string);

    // Formatando sempre em 3 casas decimais
    if(yLSB >= 100){
        lcdPrint(string);
    }
    else if(yLSB >= 10){
        lcdPrint(string);
        lcdPrint("0");
    }
    else{
        lcdPrint(string);
        lcdPrint("0");
        lcdPrint("0");
    }
    lcdPrint("V");

    // Print Hexa Y
    lcdPrint("   ");
    lcdPrint(stringHexa);
}
