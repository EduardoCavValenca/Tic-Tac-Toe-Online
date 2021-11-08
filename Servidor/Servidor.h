#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "../common/common.h"

void *envia_tabuleiro(void *);
void *recebe_tabuleiro_player1(void *);
void *recebe_tabuleiro_player2(void *);
bool verifica_vitoria(char tipo);
bool verifica_tabuleiro_cheio();
void iniciar_variaveis();

#endif