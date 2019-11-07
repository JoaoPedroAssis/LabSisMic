#include <msp430.h> 
#include <stdint.h>

void i2cInit(uint8_t isMaster, uint8_t myAddr);
uint8_t i2cTst(uint8_t addr);
uint8_t i2cPut(uint8_t addr, uint8_t data);
uint8_t i2cGet(uint8_t addr, uint8_t *data);