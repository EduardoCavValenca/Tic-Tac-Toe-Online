#include "TelaDisconnect.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)



TelaDisconnect::TelaDisconnect()
{

    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    this->texto_disconnect.setFont(this->fonte);
    this->texto_disconnect.setString("O outro jogador se desconectou");
    this->texto_disconnect.setCharacterSize(TEXT_SIZE); // em px
    this->texto_disconnect.setFillColor(TEXT_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_disconnect.getLocalBounds();
    scale = this->texto_disconnect.getScale();
    this->texto_disconnect.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 200 -(bounds.height)*scale.y/2));




    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    // this->sprite_instrucoesbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}




void TelaDisconnect::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->texto_disconnect);
}
