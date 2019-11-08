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

typedef enum {testAddr, send, receive} cmd_t;

struct {
    uint8_t status;
    uint8_t *data;
    cmd_t cmd;
} UCB0;

void wait(uint16_t time_ms);

void i2cInit(uint8_t isMaster, uint8_t myAddr);

uint8_t i2c(cmd_t cmd, uint8_t addr, uint8_t *data, uint8_t count);

#endif /* I2C_H_ */
