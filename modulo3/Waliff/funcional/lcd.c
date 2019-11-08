#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "i2c.h"

void lcdInit()
{
    if(i2cTestAddr(lcdAddr)) lcdAddr = 0x3F;

    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x2, 0);

    lcdWriteByte(0x28, 0);
    lcdWriteByte(0x0C, 0);
    lcdWriteByte(0x06, 0);

    lcdWriteByte(0x01, 0);
}

void lcdWriteNibble(uint8_t nibble, uint8_t isChar)
{
    nibble <<= 4;
    nibble |= LCDBL | isChar;
    i2cSendByte(lcdAddr, nibble | 0);
    i2cSendByte(lcdAddr, nibble | LCDEN);
    i2cSendByte(lcdAddr, nibble | 0);
}

void lcdWriteByte(uint8_t byte, uint8_t isChar)
{
    lcdWriteNibble(byte>>4, isChar);
    lcdWriteNibble(byte, isChar);
}

void lcdPrint(char* string)
{
    while(*string)
    {
        lcdWriteByte(*string++, 1);
    }
}

void lcdClear()
{
    lcdWriteByte(0x01, 0);
}
