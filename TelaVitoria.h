#ifndef TELAVITORIA_H
#define TELAVITORIA_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaVitoria : public sf::Drawable
{
private:

    sf::Font fonte;
    sf::Texture textura_vitoria;
    sf::Texture textura_background;
    sf::Sprite sprite_vitoria;
    sf::Sprite sprite_background;



public:
    TelaVitoria();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif