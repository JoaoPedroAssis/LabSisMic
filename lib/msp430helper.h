#include <msp430.h>

/** 
 * initLED
 * Inicializa um LED com um valor na saída
 * 
 * Argumentos:
 * dir - O registrador de direção da MSP430 (PxDIR)
 * out - O registrador que seta a saída da MSP430 (PxOUT)
 * bit - Bits para setar nos registradores (BITx)
 * on - Deve ser 1 ou 0, indicando o estado inicial do LED
 */
void initLED(volatile unsigned char* dir, volatile unsigned char* out, 
unsigned int bit, unsigned int on);

/**
 * initButton
 * Inicializa uma das chaves da placa (S1 ou S2)
 * 
 * Argumentos:
 * dir - O registrador de direção da MSP430 (PxDIR)
 * ren - O registrador de habilitação de resistor da MSP430 (PxREN)
 * out - O registrador que seta pull up/down na MSP430 (PxOUT)
 * bit - Bits para setar nos registradores (BITx)
 */
void initButton(volatile unsigned char* dir, volatile unsigned char* ren,
                volatile unsigned char* out, unsigned int bit) {