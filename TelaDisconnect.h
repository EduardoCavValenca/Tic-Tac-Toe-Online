#ifndef TELADISCONNECT_H
#define TELADISCONNECT_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaDisconnect : public sf::Drawable
{
private:

    sf::Font fonte;
    sf::Text texto_disconnect;
    sf::Texture textura_background;
    sf::Sprite sprite_background;


public:
    TelaDisconnect();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

};

#endif