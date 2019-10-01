#include "msp430gpio.h"


#ifndef GPIO_PORT_MAPS
const uint16_t port_to_dir[]={
    (uint16_t) &P1DIR,
    (uint16_t) &P2DIR,
    (uint16_t) &P3DIR,
    (uint16_t) &P4DIR,
    (uint16_t) &P5DIR,
    (uint16_t) &P6DIR,
    (uint16_t) &P7DIR,
    (uint16_t) &P8DIR
};

const uint16_t port_to_in[]={
    (uint16_t) &P1IN,
    (uint16_t) &P2IN,
    (uint16_t) &P3IN,
    (uint16_t) &P4IN,
    (uint16_t) &P5IN,
    (uint16_t) &P6IN,
    (uint16_t) &P7IN,
    (uint16_t) &P8IN
};

const uint16_t port_to_out[]={
    (uint16_t) &P1OUT,
    (uint16_t) &P2OUT,
    (uint16_t) &P3OUT,
    (uint16_t) &P4OUT,
    (uint16_t) &P5OUT,
    (uint16_t) &P6OUT,
    (uint16_t) &P7OUT,
    (uint16_t) &P8OUT
};

const uint16_t port_to_ren[]={
    (uint16_t) &P1REN,
    (uint16_t) &P2REN,
    (uint16_t) &P3REN,
    (uint16_t) &P4REN,
    (uint16_t) &P5REN,
    (uint16_t) &P6REN,
    (uint16_t) &P7REN,
    (uint16_t) &P8REN
};

#endif /*GPIO_PORT_MAPS*/


inline void setPin(unsigned char pin, unsigned char mode){
    uint8_t bit = Mask(pin);
    uint8_t port = Port(pin);

    volatile uint8_t *dir = portToDir(port);
    volatile uint8_t *ren = portToRen(port);
    volatile uint8_t *out = portToOut(port);

    switch(mode){
        case OUTPUT:
            *dir |= bit;
            break;
        case INPUT:
            *dir &= ~(bit);
            break;
        case INPUT_PULL_UP:
            *dir &= ~(bit);
            *ren |= bit;
            *out |= bit;
            break;
        case INPUT_PULL_DOWN:
            *dir &= ~(bit);
            *ren |= bit;
            *out &= ~(bit);
            break;
        case PULL_DOWN:
            *ren |= bit;
            *out &= ~(bit);
            break;
        case PULL_UP:
            *ren |= bit;
            *out |= bit;
            break;

        default:
            break;
    }
}

inline void writePin(unsigned char pin, unsigned char value){
    uint8_t bit = Mask(pin);
    uint8_t port = Port(pin);

    volatile uint8_t *out = portToOut(port);

    if(!value) *out &= ~(bit);
    else       *out |= bit;
}

inline void togglePin(unsigned char pin){
    uint8_t bit = Mask(pin);
    uint8_t port = Port(pin);

    volatile uint8_t *out = portToOut(port);

    *out ^= bit;
}

inline uint8_t readPin(unsigned char pin){
    uint8_t bit = Mask(pin);
    uint8_t port = Port(pin);

    volatile uint8_t *in = portToIn(port);

    return *in & bit;
}
