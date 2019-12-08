#include <msp430.h> 
#include "i2c.h"
#include "lcd.h"
#include "adc.h"

void comunic(uint8_t canal){
    uint8_t data[2];
    uint8_t *aux;
    *aux = canal;
    char string[12];
    i2c(send, 0x12, aux, 1);
    i2c(receive, 0x12, data, 2);
    itoh(data[0], string);
    lcdPrint(string);
    itoh(data[1], string);
    lcdPrint(string);
}



void main(void)
{
    uint8_t *aux;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Configurando o LED para piscar quando atualizar o LCD
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P1OUT  |= BIT0;

    // Incializa o i2c e o lcd
    i2cInit(1, 0);
    lcdInit();

    if(i2c(testAddr,0x12,0,0)) // Endereço certo
        P1OUT &= ~BIT0;

    lcdClear();
    lcdClear();
    comunic(0x02);
    while(1)
    {
      // Espera as interrupções

    }
}

/*
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR()
{

}
*/
