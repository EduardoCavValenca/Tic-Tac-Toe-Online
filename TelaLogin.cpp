#include "TelaLogin.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black
#define BACKGROUND_COLOR sf::Color(20, 189, 172)



TelaLogin::TelaLogin(string Nome)
{
    sf::FloatRect bounds;
    sf::Vector2f scale;

    this->Nome = Nome;
   

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    this->texto_menu.setFont(this->fonte);
    this->texto_menu.setString("Menu");
    this->texto_menu.setCharacterSize(TITLE_SIZE); // em px
    this->texto_menu.setFillColor(TITLE_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_menu.getLocalBounds();
    scale = this->texto_menu.getScale();
    this->texto_menu.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 30 -(bounds.height)*scale.y/2));

    this->texto_nome.setFont(this->fonte);
    this->texto_nome.setString("Username: "+ this->Nome);
    this->texto_nome.setCharacterSize(TEXT_SIZE); // em px
    this->texto_nome.setFillColor(TEXT_COLOR);
    bounds = this->texto_nome.getLocalBounds();
    scale = this->texto_nome.getScale();
    this->texto_nome.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 170 -(bounds.height)*scale.y/2));
   
    this->textura_playbutton.loadFromFile("texturas/PlayButton.png");        

    this->sprite_playbutton.setTexture(this->textura_playbutton);
    // this->sprite_playbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_playbutton.getLocalBounds();
    scale = this->sprite_playbutton.getScale();
    this->sprite_playbutton.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_playbutton.setPosition(sf::Vector2f((float) 400.0 , 330 ));


    this->textura_instrucoesbutton.loadFromFile("texturas/InstrucoesButton.png");

    this->sprite_instrucoesbutton.setTexture(this->textura_instrucoesbutton);
    // this->sprite_instrucoesbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_instrucoesbutton.getLocalBounds();
    scale = this->sprite_instrucoesbutton.getScale();
    this->sprite_instrucoesbutton.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_instrucoesbutton.setPosition(sf::Vector2f((float) 400.0 , 480 ));
    


    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    // this->sprite_instrucoesbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

void TelaLogin::ReactToMouse(sf::Vector2i Mouseposition)
{
    sf::Vector2f MouseCoord = (sf::Vector2f) Mouseposition;

    this->sprite_playbutton.setScale(0.9,0.9);
    this->sprite_instrucoesbutton.setScale(0.9,0.9);

    if(MouseCoord.x >280 && MouseCoord.x < 520)
    {
        if(MouseCoord.y > 265 && MouseCoord.y < 390)
        {
            this->sprite_playbutton.setScale(1,1);
        }

        if(MouseCoord.y > 415 && MouseCoord.y < 535)
        {
            this->sprite_instrucoesbutton.setScale(1,1);
        }
    }
}

int TelaLogin::mouseclique(sf::Vector2i Mouseposition)
{
    sf::Vector2f MouseCoord = (sf::Vector2f) Mouseposition;

    if(MouseCoord.x >280 && MouseCoord.x < 520)
    {
        if(MouseCoord.y > 265 && MouseCoord.y < 390)
        {
            return 3;
        }

        if(MouseCoord.y > 415 && MouseCoord.y < 535)
        {
            return 2;
        }
    }

    return 1;

}

void TelaLogin::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->sprite_instrucoesbutton);
    target.draw(this->sprite_playbutton);
    target.draw(this->texto_nome);
    target.draw(this->texto_menu);
}