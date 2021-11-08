#include "TelaJogo.h"

using namespace std;

#define TITLE_SIZE  60
#define TITLE_COLOR sf::Color::Black

#define TEXT_SIZE   35
#define TEXT_COLOR  sf::Color::Black

#define BACKGROUND_COLOR sf::Color(20, 189, 172)



TelaJogo::TelaJogo(char tipo_jogador, string nome_player, string nome_adversario)
{
    this->nome_player=nome_player;
    this->nome_adversario=nome_adversario;
    this->tipo_jogador=tipo_jogador;

    int i,j;

    //Inicia sem nenhum lugar marcado
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            this->marcado[i][j] = '-';
        }
    }

    this->textura_X.loadFromFile("texturas/X.png");
    this->textura_O.loadFromFile("texturas/O.png");

    this->textura_vazio.loadFromFile("texturas/Vazio.png");

    sf::FloatRect bounds;
    sf::Vector2f scale;

    //Carrega Fonte
    if(!this->fonte.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    this->texto_rodada.setFont(this->fonte);
    this->texto_rodada.setString("Rodada: "+to_string(this->num_rodadas));
    this->texto_rodada.setCharacterSize(TEXT_SIZE); // em px
    this->texto_rodada.setFillColor(TEXT_COLOR);
    // Centralizando o texto de titulo
    bounds = this->texto_rodada.getLocalBounds();
    scale = this->texto_rodada.getScale();
    this->texto_rodada.setPosition(sf::Vector2f(20.0,10.0));

    this->texto_turno.setFont(this->fonte);
    // this->texto_turno.setString("Turno: "+to_string(this->num_empates));
    this->texto_turno.setString("Turno: "+this->nome_turno);
    this->texto_turno.setCharacterSize(TEXT_SIZE); // em px
    this->texto_turno.setFillColor(TEXT_COLOR);
    this->texto_turno.setPosition(sf::Vector2f(20.0,50.0));


   
    this->textura_grid.loadFromFile("texturas/Grid.png");        

    this->sprite_grid.setTexture(this->textura_grid);
    // this->sprite_playbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_grid.getLocalBounds();
    scale = this->sprite_grid.getScale();
    this->sprite_grid.setOrigin(sf::Vector2f((float) ((bounds.width)*scale.x/ 2.0),(bounds.height)*scale.y/2));
    this->sprite_grid.setPosition(sf::Vector2f((float) 400.0 , 330 ));


    this->textura_background.loadFromFile("texturas/Fundo.png");

    this->sprite_background.setTexture(this->textura_background);
    // this->sprite_instrucoesbutton.setScale(sf::Vector2f(0.5,0.5));
    bounds = this->sprite_background.getLocalBounds();
    scale = this->sprite_background.getScale();
    this->sprite_background.setPosition(sf::Vector2f(0,0));
}

void TelaJogo::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->sprite_background);
    target.draw(this->sprite_grid);

    int i,j;

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            target.draw(this->sprite_posicoes[i][j]);
        }
    }

    target.draw(this->texto_turno);
    target.draw(this->texto_rodada);

}

int TelaJogo::mouseclique(sf::Vector2i Mouseposition, Cliente *player, int turno)
{
    if(player->tipo_jogador == 'X')
    {
        if(turno%2 != 0)
            return 0;
        
    }
    else if(player->tipo_jogador == 'O')
    {
        if(turno%2 == 0)
            return 0;
    }

    if(Mouseposition.x > 195 && Mouseposition.x <= 332) //Primeira Coluna
    {
        if(Mouseposition.y > 130 && Mouseposition.y <= 260) // (1,1)
        {
            return this->marca_posicao(0,0,player);
        }

        if(Mouseposition.y > 260 && Mouseposition.y <=395) // (2,1)
        {
            
            return this->marca_posicao(1,0,player);
            
        }

        if(Mouseposition.y > 395 && Mouseposition.y <= 530) // (3,1)
        {
            
            return this->marca_posicao(2,0,player);
        
        }
    }

    if(Mouseposition.x > 332 && Mouseposition.x <= 467) //Segunda Coluna
    {
        if(Mouseposition.y > 130 && Mouseposition.y <= 260) // (1,2)
        {
           return this->marca_posicao(0,1,player);
        }

        if(Mouseposition.y > 260 && Mouseposition.y <= 395) // (2,2)
        {
            return this->marca_posicao(1,1,player);
        }

        if(Mouseposition.y > 395 && Mouseposition.y <= 530) // (3,2)
        {
            return this->marca_posicao(2,1,player);
        }
    }

    if(Mouseposition.x > 467 && Mouseposition.x <= 600) //Terceira Coluna
    {
        if(Mouseposition.y > 130 && Mouseposition.y <= 260) // (1,3)
        {
            return this->marca_posicao(0,2,player);
        }

        if(Mouseposition.y > 260 && Mouseposition.y <= 395) // (2,3)
        {
            return this->marca_posicao(1,2,player);
        }

        if(Mouseposition.y > 395 && Mouseposition.y <= 530) // (3,3)
        {
            return this->marca_posicao(2,2,player);
        }
    }

    return 0;
}

 int TelaJogo::marca_posicao(int x, int y, Cliente *player)
 { 

    if( marcado[x][y] == '-')
    {
        this->texto_turno.setString("Turno: "+this->nome_turno);

        this->marcado[x][y] = this->tipo_jogador;
        
        player->tabuleiro[x][y] = marcado[x][y];

        return 1+x+x*y;
    }
    return 0;
}

char TelaJogo::set_tabuleiro(Cliente *player)
{
    int i,j;
    int count_novo_jogo =0;
    int count_vitoria_X =0;
    int count_vitoria_O =0;

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(player->tabuleiro[i][j] == '-') //Nov tabuleiro
                count_novo_jogo++;

            if(player->tabuleiro[i][j] == 'X')
            {
                this->sprite_posicoes[i][j].setTexture(this->textura_X);
                this->sprite_posicoes[i][j].setPosition(sf::Vector2f((float) 205.0+j*140 , 140+135*i ));
                count_vitoria_X++;
            }

            if(player->tabuleiro[i][j] == 'O')
            {
                this->sprite_posicoes[i][j].setTexture(this->textura_O);
                this->sprite_posicoes[i][j].setPosition(sf::Vector2f((float) 205.0+j*140 , 140+135*i ));
                count_vitoria_O++;
            }
            this->marcado[i][j]=player->tabuleiro[i][j];
        }
    }

    if(count_novo_jogo==9)
    {
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                this->sprite_posicoes[i][j].setTexture(this->textura_vazio);
                this->sprite_posicoes[i][j].setPosition(sf::Vector2f(0,0));
            }
        }
    }


    if(count_vitoria_X == 9)
    {
        return 'X';    
    }


    if(count_vitoria_O == 9)
    {
        return 'O';
    }

    return '-';

}

void TelaJogo::set_nome_turno(int turno)
{
    this->num_rodadas = turno+1;
    this->texto_rodada.setString("Rodada: "+to_string(this->num_rodadas));

    if(this->tipo_jogador == 'X')
    {
        if(turno%2==0) {
            this->nome_turno = this->nome_player;
        }
        else {
            this->nome_turno = this->nome_adversario;
        }
    }

    if(this->tipo_jogador == 'O')
    {
        if(turno%2==0) {
            this->nome_turno = this->nome_adversario;
        }
        else {
            this->nome_turno = this->nome_player;
        }
    }

    this->texto_turno.setString("Turno: "+this->nome_turno);
}