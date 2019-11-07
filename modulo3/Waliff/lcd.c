#include <stdint.h>
#include "lcd.h"
#include "i2c.h"

uint8_t lcdAddr = 0x27;

const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBL = 0x08;

void lcdWriteNibble(uint8_t nibble, uint8_t isChar)
{
    nibble <<= 4;                   // Ajusta a posição do nibble
    nibble |= lcdBL | isChar;       // Backlight On

    i2cPut(lcdAddr, nibble| 0 );    // Gera pulso em enable
    i2cPut(lcdAddr, nibble|lcdEN);
    i2cPut(lcdAddr, nibble| 0 );
}

void lcdWriteByte(uint8_t byte, uint8_t isChar)
{
    lcdWriteNibble(byte >> 4, isChar);
    lcdWriteNibble(byte, isChar);
}

void lcdInit()
{
    if(i2cTst(lcdAddr))             // Verifica se o LCD está no end. 0x27 ou 0x3F
        lcdAddr = 0x3F;

	lcdWriteNibble(0x3, 0);         // Entra em modo 8 bits
	lcdWriteNibble(0x3, 0);
	lcdWriteNibble(0x3, 0);
	lcdWriteNibble(0x2, 0);         // Entra em modo 4 bits
​

	lcdWriteByte(0x28, 0);          // Configura em 2 linhas
	lcdWriteByte(0x0C, 0);          // Display on, cursor on
	lcdWriteByte(0x06, 0);          // Modo de incremento

	lcdWriteByte(0x01, 0);          // Limpa a tela

}

void lcdPrint(char *str)
{
	while(*str)
		lcdWriteByte(*str++,1);

}
