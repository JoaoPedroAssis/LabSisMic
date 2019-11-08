#include <msp430.h> 
#include "i2c.h"
#include "lcd.h"

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    i2cInit(1, 0);
    __enable_interrupt();

    lcdInit();
    //lcdWriteByte(nibble, 1);

    while(1)
    {
        wait(2000);
        lcdPrint("Switch");
        wait(2000);
        lcdPrint("Dreams");
    }
}
