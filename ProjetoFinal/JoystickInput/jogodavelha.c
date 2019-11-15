/*
 * jogodavelha.c
 *
 *  Created on: 15 de nov de 2019
 *      Author: pedro
 */

#include "jogodavelha.h"
void debounce(int i){
    volatile int a;
    for(a; a < i; a++);
}
Arv* cria_vazia() {
    Arv* a= (Arv*) malloc(sizeof(Arv));
    int i,j;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            a->info[i][j]=32;
        }
    }
    a->lado=NULL;
    a->baixo=NULL;
    return a;
}

Arv* insere_simb(Arv* a,char simb) {
    int i = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    // Espera o botão
    while(P1IN & BIT6);
    debounce(10000);
    // Trocar estado do led Vermelho (Bit Toogle)
    P1OUT ^= BIT0;

    // Quando for ativado
    // Analisa jogada
    for (i = 0; i < 8; i++) {
        // Ler 8 vezes de cada canal
        x += adcRead(4);
        y += adcRead(5);
    }

    switch(joystick_jogada(x, y)) {
        case 0:
            a->info[0][0] = simb;
            break;
        case 1:
            a->info[0][1] = simb;
            break;
        case 2:
            a->info[0][2] = simb;
            break;
        case 3:
            a->info[1][0] = simb;
            break;
        case 4:
            a->info[1][1] = simb;
            break;
        case 5:
            a->info[1][2] = simb;
            break;
        case 6:
            a->info[2][0] = simb;
            break;
        case 7:
            a->info[2][1] = simb;
            break;
        case 8:
            a->info[2][2] = simb;
            break;
        default:
            a->info[2][2] = simb;
            break;
    }
    // Espera o botão ser desprecionado
    while(!(P1IN & BIT6));
    debounce(10000);
    return a;
}

int minimax(Arv* a,int jogador) {
    int winner = ganhou(a);
    if(winner != 0) return winner*jogador;
    int l,c;
    int move = -1;
    int score = -2;
    char simb;
    for(l=0;l<3;++l){
        for(c= 0; c< 3; ++c) {//Para todos movimentos,
            if(a->info[l][c]==32) {//Se tiver vago,
                if(jogador==-1)
                    simb='X';
                if(jogador==1)
                    simb='O';
                a->info[l][c] =simb;//faz o movimento
                int thisScore = -minimax(a, jogador*-1);
                if(thisScore > score) {
                    score = thisScore;
                    move = l;
                }//Pega a pior jogada para o oponente
                a->info[l][c] = 32;//Torna-o vago novamente
            }
        }
    }
    if(move == -1) return 0;
    return score;
}

int ganhou(Arv* a){
    //Retorna -1 se X ganhou,1 se O ganhou,2 se empatou,0 se nao acabou ainda
    int i,l,c,flagocol=0,flagxcol=0,flagxlinha=0,flagolinha=0,velha=0,flagxdig=0,flagodig=0;
        //Linhas e Colunas
        for(l=0;l<3;l++)
        {
            flagocol=flagxcol=flagxlinha=flagolinha=0;
            for(c=0;c<3;c++)
            {
                if(a->info[l][c]=='x'|| a->info[l][c]=='X'){
                    flagxcol++;
                    velha++;
                }
                else{
                    if(a->info[l][c]=='o'||a->info[l][c]=='O'){
                        flagocol++;
                        velha++;
                    }
                }
                if(a->info[c][l]=='x'||a->info[c][l]=='X'){
                    flagxlinha++;
                }
                else{
                    if(a->info[c][l]=='o'||a->info[c][l]=='O')
                        flagolinha++;
                }
            }
            if (flagxcol==3||flagxlinha==3){
                velha=0;
                return -1;
            }
            if (flagocol==3||flagolinha==3){
                velha=0;
                return 1;
            }
    }
    if (velha==9){
        return 0;
    }
    //Diagonal Principal
    for(l=0;l<3;l++){
        if(a->info[l][l]=='x'||a->info[l][l]=='X'){
                flagxdig++;
            }else{
                if(a->info[l][l]=='o'||a->info[l][l]=='O')
                flagodig++;
            }
    }

    if(flagodig==3){
        return 1;
    }else if(flagxdig==3){
        return -1;
    }
    //Diagonal Secundaria
    flagxdig=flagodig=0;
    if(a->info[0][2]=='x'||a->info[0][2]=='X')
        flagxdig++;
    if(a->info[1][1]=='x'||a->info[1][1]=='X')
        flagxdig++;
    if(a->info[2][0]=='x'||a->info[2][0]=='X')
        flagxdig++;
    if(a->info[0][2]=='o'||a->info[0][2]=='O')
        flagodig++;
    if(a->info[1][1]=='o'||a->info[1][1]=='O')
        flagodig++;
    if(a->info[2][0]=='o'||a->info[2][0]=='O')
        flagodig++;

    if(flagodig==3){
        return 1;
    }else if(flagxdig==3){
        return -1;
    }
    return 0;
}

Arv* vezDoPc(Arv* a,char simb){
    int linha=-1,coluna=-1;
    int score=-2;
    int l,c;
    // Para todas as opcoes de jogadas
    for(l=0;l<3;l++){
        for(c=0;c<3;c++){
            // Se estiver vago
            if(a->info[l][c]==32){
                // CPU faz a jogada
                a->info[l][c]='O';
                // A jogada eh analisada
                int tempScore = -minimax(a, -1);
                // Volta ao estado anterior esvaziando o espaco preenchido
                a->info[l][c]=32;
                // Se a jogada tiver um score maior que os anteriores
                if(tempScore > score){
                    // Adiciona a jogada como a melhor
                    score = tempScore;
                    linha = l;
                    coluna = c;
                }
            }
        }
    }
    a->info[linha][coluna]=simb;
    return a;
}

int jogoacabou(Arv* a){
        int i,l,c,flagocol=0,flagxcol=0,flagxlinha=0,flagolinha=0,velha=0,flagxdig=0,flagodig=0;
        //Linhas e Colunas
        for(l=0;l<3;l++)
        {
            flagocol=flagxcol=flagxlinha=flagolinha=0;
            for(c=0;c<3;c++)
            {
                if(a->info[l][c]=='x'|| a->info[l][c]=='X'){
                    flagxcol++;
                    velha++;
                }
                else{
                    if(a->info[l][c]=='o'||a->info[l][c]=='O'){
                        flagocol++;
                        velha++;
                    }
                }
                if(a->info[c][l]=='x'||a->info[c][l]=='X'){
                    flagxlinha++;
                }
                else{
                    if(a->info[c][l]=='o'||a->info[c][l]=='O')
                        flagolinha++;
                }
            }
            if (flagxcol==3||flagxlinha==3){
                printf("X Ganhou!\n");
                velha=0;
                return 1;
            }
            if (flagocol==3||flagolinha==3){
                printf("O Ganhou!\n");
                velha=0;
                return 1;
            }
    }
    if (velha==9){
        printf("O jogo deu velha!\n");
        return 1;
    }
    //Diagonal Principal
    for(l=0;l<3;l++)
    {
        if(a->info[l][l]=='x'||a->info[l][l]=='X'){
                flagxdig++;
            }
            else{
                if(a->info[l][l]=='o'||a->info[l][l]=='O')
                flagodig++;
            }
    }
    if(flagodig==3){
        printf("O Ganhou!\n");
        return 1;
    }
    else
    if(flagxdig==3){
        printf("X Ganhou!\n");
        return 1;
    }
    //Diagonal Secundaria
    flagodig=flagxdig=0;
    if(a->info[0][2]=='x'||a->info[0][2]=='X')
        flagxdig++;
    if(a->info[1][1]=='x'||a->info[1][1]=='X')
        flagxdig++;
    if(a->info[2][0]=='x'||a->info[2][0]=='X')
        flagxdig++;
    if(a->info[0][2]=='o'||a->info[0][2]=='O')
        flagodig++;
    if(a->info[1][1]=='o'||a->info[1][1]=='O')
        flagodig++;
    if(a->info[2][0]=='o'||a->info[2][0]=='O')
        flagodig++;
    if(flagodig==3){
        printf("O Ganhou!\n");
        return 1;
    }
    else
    if(flagxdig==3){
        printf("X Ganhou!\n");
        return 1;
    }
    return 0;
}

void print_jogo(Arv* a){
    lcdClear();
    char string[2];
    string[1] = '\0';
    // Primeira linha
    lcdPrint("0:");
    string[0] = a->info[0][0];
    lcdPrint(string);
    lcdPrint("1:");
    string[0] = a->info[0][1];
    lcdPrint(string);
    lcdPrint("2:");
    string[0] = a->info[0][2];
    lcdPrint(string);
    // 2 Termos da segunda linha
    lcdPrint("3:");
    string[0] = a->info[1][0];
    lcdPrint(string);
    lcdPrint("4:");
    string[0] = a->info[1][1];
    lcdPrint(string);

    // Pula linha
    lcdWriteByte(0xC0, 0);

    // 3 linha
    lcdPrint("6:");
    string[0] = a->info[2][0];
    lcdPrint(string);
    lcdPrint("7:");
    string[0] = a->info[2][1];
    lcdPrint(string);
    lcdPrint("8:");
    string[0] = a->info[2][2];
    lcdPrint(string);

    // Ultimo termo da segunda linha
    lcdPrint("5:");
    string[0] = a->info[1][2];
    lcdPrint(string);
}

int joystick_jogada(int somax, int somay){
    // Transforma o valor no adc no range de 0-3.3v (máximo em 12 bits)
    somax = (uint16_t)(((somax/8)*3.3/4096) * 1000);
    somay = (uint16_t)(((somay/8)*3.3/4096) * 1000);

    // Formatando print
    if (somax > 3000){ // Quadrante da direita
        if (somay < 300) { // Direita em cima
            return 2;
        } else {
            if (somay > 3000) { // Direita baixo
            return 8;
            } else { //Direita no meio
            return 5;
            }
        }
    } else {
        if (somax < 300) { // Quadrante da esquerda
            if (somay < 300) { // Esquerda em cima
            lcdPrint("0");
            } else {
                if (somay > 3000) { // Esquerda baixo
                    return 6;
                } else { //Direita no meio
                    return 3;
                }
        }
        } else { // Meiota
        if (somay < 300) { // Meio em cima
            return 1;
        } else {
            if (somay > 3000) { // Meio baixo
            return 7;
            } else { //Meio no meio
            return 4;
            }
        }
    }
}
}
