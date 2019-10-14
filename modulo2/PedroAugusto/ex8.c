#include <msp430.h>
#include "msp430gpio.h"

void debounce(unsigned int i){
    volatile int a = 0;
    for(a; a < i; a++);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    // Configurando led vermelha
    setPin(P1_0, OUTPUT);
    while(1) {
        debounce(64000);
        writePin(P1_0, HIGH);
        debounce(64000);
        writePin(P1_0, LOW);
    }
}

