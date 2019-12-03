/*
 * i2c.c
 *
 *  Created on: 8 de nov de 2019
 *      Author: pedro
 */
#include "i2c.h"

void wait(uint16_t time_ms)
{
    TB2CCR0 = time_ms << 5;
    TB2CTL  = TBSSEL__ACLK | MC__UP | TBCLR;
    while(!(TB2CTL & TBIFG));
    TB2CTL &= ~TBIFG;

}

void i2cInit(uint8_t isMaster, uint8_t myAddr)
{
    UCB0CTLW0 = UCMODE_3 | UCSWRST;         // Modo: I2C
    UCB0CTLW1 = UCASTP_2;                   // com stop automático

    //UCB0TBCNT = 1;                          // _________________________________
    if(isMaster)                            // Se for mestre:
    {                                       //
        UCB0CTLW0 |= UCSSEL__SMCLK | UCMST; // Configura como mestre e usa SMCLK
        UCB0BRW = 100;                       // SCL @ 100kHz
    }                                       // _________________________________
    else                                    // Se for escravo:
    {                                       //
        UCB0I2COA0 = UCOAEN | myAddr;       // Configura o endereço
    }                                       //
                                            // _________________________________
    P1SEL0 |=  (BIT2 | BIT3);               // Configura pinos: SDA and SCL
    P1SEL1 &= ~(BIT2 | BIT3);               //
    P1DIR  &= ~(BIT2 | BIT3);               // Habilita resistor de pull-up
    P1REN  |=  (BIT2 | BIT3);               //
    P1OUT  |=  (BIT2 | BIT3);               // _________________________________
                                            //
    UCB0CTLW0 &= ~UCSWRST;                  // Libera a interface para operar
                                            // _________________________________
                                            // Interrupções:
    UCB0IE = UCRXIE   |                     // RX
             UCTXIE   |                     // TX
             UCSTPIE  |                     // Stop
             UCNACKIE |                     // NACK
             UCBCNTIE |                     // Byte counter
             UCBIT9IE;                      // 9th clock cycle
}

uint8_t i2c(cmd_t cmd, uint8_t addr, uint8_t *data, uint8_t count)
{
    UCB0I2CSA = addr;                       // Configura end. do escravo,
    UCB0TBCNT = count;                      // o número de bytes
    UCB0.data = data;                       // e configura o ponteiro de dados



    switch (cmd) {
        case send:     UCB0CTLW0 |= UCTR + UCTXSTT;            break;
        case testAddr: UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;  break;
        case receive:
            UCB0CTLW0 &= ~UCTR;
            UCB0CTLW0 |=        UCTXSTT;
            break;
    }

    __low_power_mode_0();                   // Aguarda o final da comunicação
                                            //
    return UCB0.status;                     // retornamos 0:ACK ou 1:NACK

}

#pragma vector = USCI_B0_VECTOR
__interrupt void I2C_ISR()
{
    switch (UCB0IV)
    {
        case UCIV__UCNACKIFG:
            UCB0CTLW0 |= UCTXSTP;
            UCB0.status = 1; // NACK
            break;

        case UCIV__UCBCNTIFG:
            UCB0.status = 0; // Success
            break;

        case UCIV__UCSTPIFG:
            __low_power_mode_off_on_exit();
            break;

        case UCIV__UCRXIFG0:
            *(UCB0.data)++ = UCB0RXBUF;
            break;

        case UCIV__UCTXIFG0:

            UCB0TXBUF = *(UCB0.data)++;
            break;

        default:
            break;
    }
}
