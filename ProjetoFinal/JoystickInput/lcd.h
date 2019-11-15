#ifndef LCD_H_
#define LCD_H_

#include "i2c.h"
#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

void lcdInit();
void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
void lcdClear();
void lcdPrint (char* str);
void itos(uint16_t n, char string[6]);
void itoh(uint16_t n, char string[5]);
void ldcPrintVoltagem(int soma, char* nome);

#endif /* LCD_H_ */
