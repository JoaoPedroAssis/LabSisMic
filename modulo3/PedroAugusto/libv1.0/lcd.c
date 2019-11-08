/*
 * lcd.c
 *
 *  Created on: 8 de nov de 2019
 *      Author: pedro
 */
#include "lcd.h"

uint8_t lcdAddr = 0x27;

const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBL = 0x08;

void lcdInit() {
    // Testa o endereço do PCF/LCD
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
    uint8_t aux;
    nibble <<= 4;
    nibble |= lcdBL | isChar ;
    aux = nibble | 0;
    //nibble |= 0;
    //nibble &= ~(lcdEN);
    i2c(send, lcdAddr, &aux, 1);
    nibble |= lcdEN;
    i2c(send, lcdAddr, &nibble, 1);
    nibble |= 0;
    //nibble &= ~(lcdEN);
    i2c(send, lcdAddr, &aux, 1);
}

void lcdWriteByte(uint8_t byte, uint8_t isChar){
    lcdWriteNibble (byte >> 4, isChar);
    lcdWriteNibble (byte , isChar);
}

void lcdPrint (char* str){
    while(*str)
        lcdWriteByte(*str++, 1);
}
