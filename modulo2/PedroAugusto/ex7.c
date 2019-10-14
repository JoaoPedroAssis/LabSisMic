#include <msp430.h>
#include "msp430gpio.h"

unsigned char rand() {
    static unsigned char num = 5;
    num = (num * 17) % 7;
    return num;
}

void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;

    // Configurando botão
    setPin(P4_1, INPUT_PULL_UP); //S1
    setPin(P2_3, INPUT_PULL_UP); //S2
    // Configurando led vermelha
    setPin(P1_0, OUTPUT);
    // Configurando led verde
    setPin(P6_6, OUTPUT);
    int cont = 0;
    unsigned char rand_int = rand();
    while(1) {
        while(readPin(P4_1) == UNPRESSED || readPin(P2_3) == UNPRESSED);
        
        debounce(1000);
        if (readPin(P2_3) == PRESSED) {
            if (rand_int == cont) {
                writePin(P6_6, HIGH);
                writePin(P1_0, LOW);
                cont = 0;
                rand_int = rand();
            } else {
                writePin(P1_0, HIGH);
                writePin(P6_6, LOW);
                cont = 0;
            }
        }

        if (readPin(P4_1) == PRESSED)
            cont += 1;

        while(readPin(P4_1) == PRESSED || readPin(P2_3) == PRESSED);


        }
    }

