#include "TelaAguardo.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)

TelaAguardo::TelaAguardo()
{
    //Counter para realizar animacao da pagina
    this->counter = 0;

    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    //Seta texto de aguardo
    this->texto_aguardo.setFont(this->fonte);
    this->texto_aguardo.setString("Aguardando segundo jogador");
    this->texto_aguardo.setCharacterSize(TEXT_SIZE); // em px
    this->texto_aguardo.setFillColor(TEXT_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_aguardo.getLocalBounds();
    scale = this->texto_aguardo.getScale();
    this->texto_aguardo.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 200 -(bounds.height)*scale.y/2));

   
    //Carrega a textura e seta o botao
    this->textura_VoltarButton.loadFromFile("texturas/VoltarButton.png");

    this->sprite_VoltarButton.setTexture(this->textura_VoltarButton);
    this->sprite_VoltarButton.setScale(sf::Vector2f(0.9,0.9));
    bounds = this->sprite_VoltarButton.getLocalBounds();
    scale = this->sprite_VoltarButton.getScale();
    this->sprite_VoltarButton.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_VoltarButton.setPosition(sf::Vector2f((float) 390.0 , 400 ));

    //Defini Fundo
    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

//Caso jogador clique em voltar
int TelaAguardo::mouseclique(sf::Vector2i Mouseposition)
{
    sf::Vector2f MouseCoord = (sf::Vector2f) Mouseposition;

    if(MouseCoord.x >300 && MouseCoord.x < 500)
    {
        if(MouseCoord.y > 355 && MouseCoord.y < 455)
        {
            return 1;
        }
    }

    return 3;
}

//Botao responsivo
void TelaAguardo::ReactToMouse(sf::Vector2i Mouseposition)
{
    sf::Vector2f MouseCoord = (sf::Vector2f) Mouseposition;

    this->sprite_VoltarButton.setScale(0.9,0.9);

   if(MouseCoord.x >300 && MouseCoord.x < 500)
    {
        if(MouseCoord.y > 355 && MouseCoord.y < 455)
        {
            this->sprite_VoltarButton.setScale(1,1);
        }
    }
}

// Função de animação do texto de espera
void TelaAguardo::change_text()
{
    this->counter+=1;

    if(this->counter == 100) this->texto_aguardo.setString("Aguardando segundo jogador .");
    if(this->counter == 200) this->texto_aguardo.setString("Aguardando segundo jogador . .");
    if(this->counter == 300) this->texto_aguardo.setString("Aguardando segundo jogador . . .");
    if(this->counter == 400) 
    {
        this->texto_aguardo.setString("Aguardando segundo jogador");
        this->counter=0;
    }
}

// Desenhando tela de aguardo
void TelaAguardo::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->texto_aguardo);
    target.draw(this->sprite_VoltarButton);
}