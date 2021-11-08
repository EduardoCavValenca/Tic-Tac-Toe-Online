#ifndef TELADERROTA_H
#define TELADERROTA_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaDerrota : public sf::Drawable
{
private:

    sf::Font fonte;
    sf::Texture textura_derrota;
    sf::Texture textura_background;
    sf::Sprite sprite_derrota;
    sf::Sprite sprite_background;



public:
    TelaDerrota();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif