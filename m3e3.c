#include <msp430.h> 
#include <stdint.h>

void wait(uint16_t time_ms);
uint8_t i2cTestAddr(uint8_t addr);
uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cSendByte(uint8_t addr, uint8_t byte);
uint8_t i2cGetByte(uint8_t addr, uint8_t byte);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Config
    UCB0CTLW0 = UCSSEL__SMCLK | UCMST | UCMODE_3 | UCSWRST;
    UCB0BRW   = 10;

    P1SEL0 |=  (BIT2 | BIT3);
    P1SEL1 &= ~(BIT2 | BIT3);
    P1DIR  &= ~(BIT2 | BIT3);
    P1REN  |=  (BIT2 | BIT3);
    P1OUT  |=  (BIT2 | BIT3);

    UCB0CTLW0 &= ~UCSWRST;

    uint8_t lcdAddr = 0x27;

    // Testa o endereço do PCF/LCD
    if(i2cTestAddr(lcdAddr))
        lcdAddr = 0x3F;

    while(1)
    {
        wait(1000);
        i2cSendByte(lcdAddr, 0x08);
        wait(1000);
        i2cSendByte(lcdAddr, 0x00);
    }
}

void wait(uint16_t time_ms)
{
    TB2CCR0 = time_ms << 5;
    TB2CTL  = TBSSEL__ACLK | MC__UP | TBCLR;
    while(!(TB2CTL & TBIFG));
    TB2CTL &= ~TBIFG;

}

uint8_t i2cTestAddr(uint8_t addr)
{
    UCB0IFG = 0;                            // Zera as flags, pois NACK não
                                            // é zerado no start

    UCB0I2CSA  = addr;                      // Configura end. do escravo
    UCB0CTLW0 |= UCTR + UCTXSTT;            // Pede um start

    while(!(UCB0IFG & UCTXIFG));            // Escreve um byte dummy para
    UCB0TXBUF = 0x00;                       // destravar o ciclo de ACK

    while( !(UCB0IFG & UCNACKIFG) &&        // Espera ou NACK
           !(UCB0IFG & UCTXIFG) );          // ou TXIFG

    UCB0CTLW0 |= UCTXSTP;                   // Pede o stop e
    while(UCB0CTLW0 & UCTXSTP);             // aguarda ele ser enviado.

    return (UCB0IFG & UCNACKIFG);           // Retorna ACK ou NACK
}

uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count)
{
    UCB0IFG = 0;                            // Zera as flags, pois NACK não
                                            // é zerado no start

    UCB0I2CSA  = addr;                      // Configura end. do escravo
    UCB0CTLW0 |= UCTR + UCTXSTT;            // Pede um start

    while(!(UCB0IFG & UCTXIFG));            // Escreve o primeiro byte
    UCB0TXBUF = *data++;                    // p/ destravar o ciclo de ACK
    count--;

    while( !(UCB0IFG & UCNACKIFG) &&        // Espera ou NACK
           !(UCB0IFG & UCTXIFG) );          // ou TXIFG

    if (UCB0IFG & UCNACKIFG)
    {                                       // Se for um NACK,
        UCB0CTLW0 |= UCTXSTP;               // devemos pedir um stop,
        while(UCB0CTLW0 & UCTXSTP);         // aguardar o envio do mesmo.
        return 1;                           // e retornamos um código de erro
    }

    while(count--)                          // Continuamos com o envio
    {                                       // dos bytes restantes
        while(!(UCB0IFG & UCTXIFG));        //
        UCB0TXBUF = *data++;                //
    }

    UCB0CTLW0 |= UCTXSTP;                   // Tendo enviado todos os bytes,
    while(UCB0CTLW0 & UCTXSTP);             // pedimos o envio de um stop
    return 0;                               // retornamos o código de sucesso

}

uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count)
{
    UCB0I2CSA  = addr;                      // Configura end. do escravo
    UCB0CTLW0 &= ~UCTR;                     // Pede um start como receptor
    UCB0CTLW0 |= UCTXSTT;                   //

    while(UCB0IFG & UCTXSTT);               // Espera o ciclo de ACK
                                            // (não funciona na FR2355)
    if (UCB0IFG & UCNACKIFG)
    {                                       // Se for um NACK,
        UCB0CTLW0 |= UCTXSTP;               // devemos pedir um stop,
        while(UCB0CTLW0 & UCTXSTP);         // aguardar o envio do mesmo.
        return 1;                           // e retornamos um código de erro
    }

    while(--count)
    {
        while(!(UCB0IFG & UCRXIFG));        // Realizamos a recepção de
        *data++ = UCB0RXBUF;                // n-1 bytes
    }

    UCB0CTLW0 |= UCTXSTP;                   // Antes do último byte,
                                            // requisitamos um stop

    while(!(UCB0IFG & UCRXIFG));            // Só então fazemos a leitura do
    *data++ = UCB0RXBUF;                    // último byte

    while(UCB0CTLW0 & UCTXSTP);             // aguardardamos o envio do stop.

    return 0;                               // retornamos o código de sucesso

}


uint8_t i2cSendByte(uint8_t addr, uint8_t byte)
{
    return i2cSend(addr,&byte,1);
}

uint8_t i2cGetByte(uint8_t addr, uint8_t byte)
{
    return i2cGet(addr,&byte,1);
}
