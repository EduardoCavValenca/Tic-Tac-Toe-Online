#ifndef TELAAGUARDO_H
#define TELAAGUARDO_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaAguardo : public sf::Drawable
{
private:
    int counter;
    sf::Font fonte;
    sf::Text texto_aguardo;
    sf::Texture textura_background;
    sf::Sprite sprite_background;

    sf::Texture textura_VoltarButton;
    sf::Sprite sprite_VoltarButton;

public:
    TelaAguardo();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    void change_text();
    int mouseclique(sf::Vector2i Mouseposition);
    void ReactToMouse(sf::Vector2i Mouseposition);
};

#endif