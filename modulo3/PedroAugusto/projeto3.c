#include <msp430.h> 
#include <stdint.h>

typedef enum {testAddr, send, receive} cmd_t;

uint8_t lcdAddr = 0x27;
const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBT = 0x08;

void lcdInit();
void wait(uint16_t time_ms);
void i2cInit(uint8_t isMaster, uint8_t myAddr);
uint8_t i2c(cmd_t cmd, uint8_t addr, uint8_t *data, uint8_t count);
void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
void lcdPrint (char* str);

struct {
    uint8_t status;
    uint8_t *data;
    cmd_t cmd;
} UCB0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    uint8_t lcdAddr = 0x27;

    i2cInit(1, 0);

    // Testa o endereço do PCF/LCD
    if(i2c(testAddr,lcdAddr,0,0))
        lcdAddr = 0x3F;
    lcdInit();
    wait(1000);
    char *vetor = "ABC";
    lcdPrint(vetor);
    /*while(1){
        lcdWriteByte(0xFF, 1);
        wait(1000);
    }*/
    /*
    while(1)
    {

        uint8_t vetor = 0x0F;

        wait(1000);

        lcdWriteByte(vetor, 1);
        wait(1000);
    }*/
}

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
                                            // _________________________________
    if(isMaster)                            // Se for mestre:
    {                                       //
        UCB0CTLW0 |= UCSSEL__SMCLK | UCMST; // Configura como mestre e usa SMCLK
        UCB0BRW = 10;                       // SCL @ 100kHz
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

    UCB0CTLW0 &= ~UCTR;

    switch (cmd) {
        case send:     UCB0CTLW0 |= UCTR + UCTXSTT;            break;
        case testAddr: UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;  break;
        case receive:  UCB0CTLW0 |=        UCTXSTT;            break;
    }

    __low_power_mode_0();                   // Aguarda o final da comunicação
                                            //
    return UCB0.status;                     // retornamos 0:ACK ou 1:NACK

}



void lcdInit() {
    if(i2c(testAddr,lcdAddr,0,0))
        lcdAddr = 0x3F;

    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x2, 0);

    lcdWriteByte(0x28, 0); // 4 bits e 2 linhas
    lcdWriteByte(0x0C, 0);
    lcdWriteByte(0x06, 0);

    lcdWriteByte(0x01, 0);
}

void lcdWriteNibble(uint8_t nibble, uint8_t isChar){
    nibble <<= 4;
    nibble |= lcdBT | isChar;

    i2c(send, lcdAddr, nibble|0, 1);
    i2c(send, lcdAddr, nibble|lcdEN, 1);
    i2c(send, lcdAddr, nibble|0, 1);
}

void lcdWriteByte(uint8_t byte, uint8_t isChar){
    lcdWriteNibble (byte >> 4, isChar);
    lcdWriteNibble (byte , isChar);
}

void lcdPrint (char* str){
    while(*str)
        lcdWriteByte(*str++, 1);
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
