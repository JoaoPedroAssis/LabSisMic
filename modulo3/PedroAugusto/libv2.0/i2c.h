/*
 * i2c.h
 *
 *  Created on: 8 de nov de 2019
 *      Author: pedro
 */

#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>

void wait(uint16_t time_ms);
uint8_t i2cTestAddr(uint8_t addr);
uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cSendByte(uint8_t addr, uint8_t byte);
uint8_t i2cGetByte(uint8_t addr, uint8_t byte);

#endif /* I2C_H_ */
