#include "TelaDerrota.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)

TelaDerrota::TelaDerrota()
{
    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    //Carrega e seta imagem de derrota
    this->textura_derrota.loadFromFile("texturas/Derrota.png");

    this->sprite_derrota.setTexture(this->textura_derrota);
    bounds = this->sprite_derrota.getLocalBounds();
    scale = this->sprite_derrota.getScale();
    this->sprite_derrota.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_derrota.setPosition(sf::Vector2f((float) 400.0 , 250 ));


    //Seta fundo da tela
    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

// Desenha tela de derrota
void TelaDerrota::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->sprite_derrota);
}
