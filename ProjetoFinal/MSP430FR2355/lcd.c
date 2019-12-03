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

void lcdClear()
{
    lcdWriteByte(0x01, 0);
}

void itos(uint16_t n, char string[6])
{
    char const digit[] = "0123456789";
    char* p = string;
    int shifter = n;
    do{ // Vai até o final da string
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ // Volta preenchendo o vetor
        *--p = digit[n%10];
        n = n/10;
    }while(n);
}

void itoh(uint16_t n, char string[5]){
    string[0] = TO_HEX(((n & 0xF000) >> 12));
    string[1] = TO_HEX(((n & 0x0F00) >> 8));
    string[2] = TO_HEX(((n & 0x00F0) >> 4));
    string[3] = TO_HEX((n & 0x000F));
    string[4] = '\0';
}

void ldcPrintVoltagem(int soma, char* nome){
    char string[6];
    char stringHexa[5];
    uint16_t soma_int, soma_frac;

    // Coletando a String com o hexadecimal
    itoh((uint16_t) (soma/8), stringHexa);

    // Transforma o valor no adc no range de 0-3.3v (máximo em 12 bits)
    soma = (uint16_t)(((soma/8)*3.3/4096) * 1000);

    // Para o float separando a francionaria e a inteira
    soma_int = soma/1000;
    soma_frac = soma%1000;

    // Formatando print
    lcdPrint(nome);
    // Converta a parte inteira para string
    itos((soma_int), string);
    // Printa a parte inteira
    lcdPrint(string);
    // Coloca um ponto para divir a parte inteira da fracionária
    lcdPrint(".");
    // Converte a parte fracionária
    itos((soma_frac), string);

    // colocar sempre com 3 casas decimais a parte fracionária
    if (soma_frac >= 100) {
        lcdPrint(string);
    }
    else if (soma_frac >= 10) {
        lcdPrint(string);
        lcdPrint("0");
    }
    else
    {
        lcdPrint(string);
        lcdPrint("0");
        lcdPrint("0");
    }
    lcdPrint("V");

    // Print Hexa
    lcdPrint("   ");
    lcdPrint(stringHexa);
}

