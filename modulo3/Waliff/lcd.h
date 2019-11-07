#include <msp430.h> 
#include <stdint.h>

void lcdInit();
void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
void lcdPrint(char *str);