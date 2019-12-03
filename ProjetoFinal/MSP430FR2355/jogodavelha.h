/*
 * jogodavelha.h
 *
 *  Created on: 15 de nov de 2019
 *      Author: pedro
 */

#ifndef JOGODAVELHA_H_
#define JOGODAVELHA_H_

#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"

typedef struct node
{
    char info[3][3];
    struct node *lado;
    struct node *baixo;
}Arv;

//Funcoes para utilizacao de arvore
Arv* cria_vazia();
Arv* insere_simb(Arv* a,char simb);
//Funcoes para estrategia MiniMax
int minimax(Arv* a,int jogador);
int ganhou(Arv* a);

//Funcoes para adiministrar o andamento do jogo
int switch_setinha();
Arv* vezDoPc(Arv* a,char simb);
Arv* vezDoPcAleatoria(Arv* a,char simb);
int jogoacabou(Arv* a);

//Funcao para interface com usuario
void print_jogo(Arv* a);

// JoyStick
int joystick_jogada(int somax, int somay);

#endif /* JOGODAVELHA_H_ */
