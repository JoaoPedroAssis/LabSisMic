#ifndef LCD_H_
#define LCD_H_

#include <msp430.h>
#include <stdint.h>


void i2cInit();
uint8_t i2cTestAddr(uint8_t addr);
uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cSendByte(uint8_t addr, uint8_t byte);
uint8_t i2cGetByte(uint8_t addr, uint8_t byte);
inline void wait(volatile uint16_t time) {
    while(time--);
}

#endif /* LCD_H_ */
