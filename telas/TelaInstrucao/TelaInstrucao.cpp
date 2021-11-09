#include "TelaInstrucao.h"

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)

TelaInstrucao::TelaInstrucao()
{
    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    //Defini titulo da pagina
    this->texto_instrucao.setFont(this->fonte);
    this->texto_instrucao.setString("Como Jogar");
    this->texto_instrucao.setCharacterSize(TITLE_SIZE); // em px
    this->texto_instrucao.setFillColor(TITLE_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_instrucao.getLocalBounds();
    scale = this->texto_instrucao.getScale();
    this->texto_instrucao.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 50 -(bounds.height)*scale.y/2));

    //Defini frase da linha 1
    this->texto_dica1.setFont(this->fonte);
    this->texto_dica1.setString("Cada jogador joga 1 turno por vez");
    this->texto_dica1.setCharacterSize(TEXT_SIZE); // em px
    this->texto_dica1.setFillColor(TITLE_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_dica1.getLocalBounds();
    scale = this->texto_dica1.getScale();
    this->texto_dica1.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 200 -(bounds.height)*scale.y/2));

    //Defini frase da linha 2
    this->texto_dica2.setFont(this->fonte);
    this->texto_dica2.setString("Vence o primeiro jogador a alinhar 3 marcas");
    this->texto_dica2.setCharacterSize(TEXT_SIZE); // em px
    this->texto_dica2.setFillColor(TITLE_COLOR);
    
    // Centralizando o texto de titulo
    bounds = this->texto_dica2.getLocalBounds();
    scale = this->texto_dica2.getScale();
    this->texto_dica2.setPosition(sf::Vector2f((float) (400.0 - (bounds.width)*scale.x/ 2.0), 250 -(bounds.height)*scale.y/2));

    //Carrega botao
    this->textura_VoltarButton.loadFromFile("texturas/VoltarButton.png");

    this->sprite_VoltarButton.setTexture(this->textura_VoltarButton);
    this->sprite_VoltarButton.setScale(sf::Vector2f(0.9,0.9));
    bounds = this->sprite_VoltarButton.getLocalBounds();
    scale = this->sprite_VoltarButton.getScale();
    this->sprite_VoltarButton.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_VoltarButton.setPosition(sf::Vector2f((float) 390.0 , 400 ));


    //Defini fundo da tela
    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

// Verifica se clicou em voltar
int TelaInstrucao::mouseclique(sf::Vector2i Mouseposition)
{
    sf::Vector2f MouseCoord = (sf::Vector2f) Mouseposition;

    if(MouseCoord.x >300 && MouseCoord.x < 500)
    {
        if(MouseCoord.y > 355 && MouseCoord.y < 455)
        {
            return 1;
        }
    }

    return 2;
}

//Botao responsivo
void TelaInstrucao::ReactToMouse(sf::Vector2i Mouseposition)
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

// Desenha tela de instrução
void TelaInstrucao::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->texto_instrucao);
    target.draw(this->texto_dica1);
    target.draw(this->texto_dica2);
    target.draw(this->sprite_VoltarButton);
}