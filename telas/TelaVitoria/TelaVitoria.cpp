#include "TelaVitoria.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)

TelaVitoria::TelaVitoria()
{
    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    //Carrega imagem de vitoria
    this->textura_vitoria.loadFromFile("texturas/Vitoria.png");

    this->sprite_vitoria.setTexture(this->textura_vitoria);
    bounds = this->sprite_vitoria.getLocalBounds();
    scale = this->sprite_vitoria.getScale();
    this->sprite_vitoria.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_vitoria.setPosition(sf::Vector2f((float) 400.0 , 250 ));

    //Defini fundo da tela
    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

// Desenha tela de vitoria
void TelaVitoria::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->sprite_vitoria);
}
