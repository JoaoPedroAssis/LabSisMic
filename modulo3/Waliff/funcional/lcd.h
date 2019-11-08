#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>

#define LCDRS 0x01
#define LCDR 0x02
#define LCDEN 0x04
#define LCDBL 0x08
static uint8_t lcdAddr = 0x27;

void lcdInit();
void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
void lcdPrint(char* string);
void lcdClear();


#endif /* I2C_H_ */
