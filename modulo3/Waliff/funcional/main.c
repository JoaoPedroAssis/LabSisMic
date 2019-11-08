#include <msp430.h> 
#include <stdint.h>
#include "i2c.h"
#include "lcd.h"


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // Config
    i2cInit();
    lcdInit();
    lcdPrint("Bora Bora!!");

    wait(10000);


    /*while(1)
    {
        wait(1000);
        i2cSendByte(lcdAddr, 0x08);
        wait(1000);
        i2cSendByte(lcdAddr, 0x00);
    }*/



}
