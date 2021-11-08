#ifndef TELALOGIN_H
#define TELALOGIN_H

using namespace std;
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

class TelaLogin : public sf::Drawable
{
private:
    string Nome;
    sf::Font fonte;
    sf::Text texto_menu;
    sf::Text texto_nome;
    sf::Texture textura_playbutton;
    sf::Texture textura_instrucoesbutton;
    sf::Texture textura_background;
    sf::Sprite sprite_playbutton;
    sf::Sprite sprite_instrucoesbutton;
    sf::Sprite sprite_background;

public:
    TelaLogin(string Nome);
    void ReactToMouse(sf::Vector2i Mouseposition);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    int mouseclique(sf::Vector2i Mouseposition);
};

#endif