#include <msp430.h> 
#include <stdint.h>
#include "i2c.h"

struct {
    uint8_t status;
    uint8_t data;
} UCB0;

void i2cInit(uint8_t isMaster, uint8_t myAddr)
{
    UCB0CTLW0 = UCMODE_3 | UCSWRST;         // Modo I2C e reset
    UCB0CTLW1 = UCASTP_2;                   // Stop automático
    UCB0TBCNT = 1;                          // Recepção de 1 byte

    if(isMaster)
    {
        UCB0CTLW0 |= UCSSEL__SMCLK | UCMST; // Configura o mestre e usa SMCLK
        UCB0BRW = 10;                       // SCL @100kHz
    }
    else
    {
        UCB0I2COA0 = UCOAEN | myAddr;       // Configura o endereço
    }

    P1SEL0 |= (BIT2 | BIT3);                // Configura os pinos
    P1SEL1 &= ~(BIT2 | BIT3);               // SDA (P1.2) e SCL (P1.3)
    P1DIR &= ~(BIT2 | BIT3);                // Habilita resistor de pull-up
    P1REN |= (BIT2 | BIT3);
    P1OUT |= (BIT2 | BIT3);

    UCB0CTLW0 &= ~UCSWRST;                  // Zerando o bit de reset

                                            // Interrupções
    UCB0IE = UCRXIE     |                   // RX - recepção
             UCTXIE     |                   // TX - transmissão
             UCSTPIE    |                   // STOP
             UCNACKIE;                      // NACK
}

uint8_t i2cTst(uint8_t addr)
{
    UCB0.status = 0;                        // Consideramos que a transmissão será bem sucedida

    UCB0I2CSA = addr;                       // Configura o endereço do escravo 
    UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;  // Pede start e stop simultaneamente

    __low_power_mode_0();                   // Aguarda o final da comunicação
    return UCB0.status;                     // Return 0 se ACK ou 1 se NACK
}

uint8_t i2cPut(uint8_t addr, uint8_t data)
{
    UCB0.status = 0;                // Consideramos que a transmissão será sucedida

    UCB0I2CSA = addr;               // Configura o endereço do escravo
    UCB0.data = data;               // copia o byte para a interface

    UCB0CTLW0 |= UCTR + UCTXSTT;    // Pede inicio da comunicação

    __low_power_mode_0();           // Aguarda o final da comunicação

    return UCB0.status;             // Return 0 se ACK ou 1 se NACK
}

uint8_t i2cGet(uint8_t addr, uint8_t *data)
{
    UCB0.status = 0;                // Consideramos que a transmissão será bem sucedida

    UCB0I2CSA = addr;               // Configura o endereço do escravo

    UCB0CTLW0 &= ~UCTR;             // Pede start como receptor (não transmissor)
    UCB0CTLW |= UCTXSTT;     

    __low_power_mode_0();           // Aguarda o final da comunicação   

    *data = UCB0.data;              // Grava o byte no ponteiro do usuário

    return UCB0.status;             // Return 0 se ACK ou 1 se NACK
    
}

#pragma vector = USCI_B0_VECTOR
__interrupt void I2C_ISR()
{
    switch(UCB0IV)                          // Vetor de interrupções da eUSCI
    {
        case UCIV__UCNACKIFG:               // Se recebeu NACK
            UCB0CTLW0 |= UCTXSTP;           // Pede stop
            UCB0.status = 1;                // Avisa o usuário do NACK
            break;
        
        case UCIV__UCSTPIFG:                // Se enviou o stop
            __low_power_mode_off_on_exit()  // Volta para o programa principal
            break;                          // Saindo do modo de baixo consumo
        
        case UCIV__UCRXIFG0:                // Se recebeu algo no buffer de recepção
            UCB0.data = UCB0RXBUF;          // Escreve no ponteiro do usuário
            break;
        
        case UCIV__UCTXIFG0:                // Se for transmitir algo, pega o byte indicado pelo
            UCB0TXBUF = UCB0.data;          // ponteiro do usuário
            break;
        
        default:
            break;
    }
}