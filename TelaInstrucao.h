#ifndef TELAINSTRUCAO_H
#define TELAINSTRUCAO_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaInstrucao : public sf::Drawable
{
private:

    sf::Font fonte;

    sf::Text texto_instrucao;
    sf::Text texto_dica1;
    sf::Text texto_dica2;

    sf::Texture textura_background;
    sf::Sprite sprite_background;

    sf::Texture textura_VoltarButton;
    sf::Sprite sprite_VoltarButton;

public:
    TelaInstrucao();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    int mouseclique(sf::Vector2i Mouseposition);
    void ReactToMouse(sf::Vector2i Mouseposition);
};

#endif