#include <msp430.h>
#include <msp430gpio.h>

uint16_t getPerido();


int main(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    disableHighZ;               // Retirando a alta impedancia dos pinos

    setPin(P6_0, OUTPUT);       // S0 do sensor
    writePin(P6_0, HIGH);

    setPin(P6_1, OUTPUT);       // S1 do sensor
    writePin(P6_1, LOW);

    setPin(P6_2, OUTPUT);       // S2 do sensor
    writePin(P6_2, HIGH);

    setPin(P6_3, OUTPUT);       // S3 do sensor
    writePin(P6_3, LOW);

    setPin(P6_4, OUTPUT);       // Output Enable do sensor
    writePin(P6_4, LOW);

    setPin(P3_7, INPUT);        //Output do sensor

    setPin(P1_0, OUTPUT);       //Led Vermelho
    writePin(P1_0, LOW);

    setPin(P6_6, OUTPUT);       //Led Verde
    writePin(P6_6, LOW);



    TB0CTL =    TBSSEL__SMCLK     |   // Ajustando timer
                    MC__CONTINUOUS|
                    TBCLR;

    int16_t tRed;
    int16_t tGreen;
    int16_t tBlue;


    while (1) {
        writePin(P6_2, LOW);    //colocando sensor para o red
        writePin(P6_3, LOW);

        tRed = getPerido();

        writePin(P6_2, HIGH);    //colocando sensor para o red
        writePin(P6_3, HIGH);

        tGreen = getPerido();

        writePin(P6_2, LOW);    //colocando sensor para o red
        writePin(P6_3, HIGH);

        tBlue = getPerido();

        if(tRed < tGreen){
            if(tRed < tBlue){
                writePin(P1_0, HIGH);
                writePin(P6_6, LOW);
            } else {
                writePin(P1_0, HIGH);
                writePin(P6_6, HIGH);
            }
        } else {
            if(tGreen < tBlue){
                writePin(P1_0, LOW);
                writePin(P6_6, HIGH);
            } else {
                writePin(P1_0, HIGH);
                writePin(P6_6, HIGH);
            }

        }
        TB0CTL |= TBCLR;
        while(!(TB0CTL & BIT0));
        TB0CTL &= ~(BIT0);


    }







    return 0;
}

uint16_t getPerido() {
    uint16_t tInicial = 0, tFinal = 0;
    uint8_t i,tamanho;
    tamanho = 10;
    int16_t vecPeriodo[10];

    while(readPin(P3_7) == HIGH);

    for(i = 0;i < tamanho;i++) {
        while(readPin(P3_7) == LOW);
        tInicial = tFinal;
        tFinal = TB0R;
        vecPeriodo[i] = tFinal - tInicial;
        if(vecPeriodo[i] < 0){
            vecPeriodo[i] += 65536;
        }
        while(readPin(P3_7) == HIGH);
    }

    uint16_t media = 0;
    for(i = 1;i < tamanho;i++){
        media += vecPeriodo[i]/(tamanho - 1);
    }
    return media;

}
