#ifndef TELAJOGO_H
#define TELAJOGO_H

using namespace std;
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "../../Cliente/Cliente.h"

class TelaJogo : public sf::Drawable
{
private:

    int num_rodadas = 1;
    int num_vitorias = 0;
    int num_derrotas = 0;
    int num_empates = 0;

    char tipo_jogador;

    string nome_turno = "";

    string nome_player;
    string nome_adversario;


    sf::Font fonte;
    sf::Text texto_turno;
    sf::Text texto_rodada;

    sf::Texture textura_grid;
    sf::Texture textura_X;
    sf::Texture textura_O;
    sf::Texture textura_background;

    sf::Texture textura_vazio;

    sf::Sprite sprite_grid;
    sf::Sprite sprite_posicoes[3][3];
    sf::Sprite sprite_background;

public:
    char marcado[3][3];
    TelaJogo(char tipo_jogador,string nome_player, string nome_adversario);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    int mouseclique(sf::Vector2i Mouseposition,Cliente* player,int turno);
    int marca_posicao(int x, int y, Cliente* player);
    char set_tabuleiro(Cliente *player);
    void set_nome_turno(int turno);
};

#endif