#include <iostream>
#include <SFML/Graphics.hpp>
#include "TelaLogin.h"
#include "TelaAguardo.h"
#include "TelaDisconnect.h"
#include "TelaDerrota.h"
#include "TelaVitoria.h"
#include "TelaJogo.h"
#include "TelaInstrucao.h"
#include "Cliente.h"


int verifica_vitoria(char vitoria, Cliente*player)
{
    
    if(vitoria == 'X' && player->tipo_jogador == 'X')
    {
        return 5;
    }    

    if(vitoria == 'X' && player->tipo_jogador == 'O')
    {
        return 6;
    }   

    if(vitoria == 'O' && player->tipo_jogador == 'X')
    {
        return 6;
    }

    if(vitoria == 'O' && player->tipo_jogador == 'O')
    {
        return 5;
    }   

    return 4;
}


int main(int argc, char *argv[])
{    
    std::thread aguarda_player2;
    std::thread recebe_tabuleiro;
    std::thread envia_tabuleiro;
    bool wait_for_player2 = false;
    bool iniciou_jogo = false;
    int turno = 0;
    char verifica_vitoria_char = '-';
    char sleep;
    int count_sleep=0;
    
    string Nome;
    std::cout << "Digite seu Nome" << std::endl;
    std::getline(cin,Nome);

    Cliente* player = new Cliente(Nome);

    int JanelaAtual = 1;

    // Criando configuracoes de contexto para ter formas com melhores contornos
    sf::ContextSettings settings;
    settings.antialiasingLevel = 20;

    // Criando a janela do jogo
    sf::RenderWindow window(
        sf::VideoMode(800, 600), "Jogo da Velha", sf::Style::Titlebar | sf::Style::Close, settings
    );
    window.setPosition(sf::Vector2i(100, 50));

    // Importando icone da aplicacao
    sf::Image icon;
    if(!icon.loadFromFile("texturas/Icon.png")) {
        std::cout << "vixi" << std::endl;
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    bool mouse_clicado = false;
    sf::Vector2i Mouse_clique_position;
    sf::Vector2i Mouseposition;


    TelaLogin* tela_login = new TelaLogin(Nome); //1
    TelaInstrucao* tela_instrucao = new TelaInstrucao(); //2
    TelaAguardo* tela_aguardo = new TelaAguardo(); //3
    TelaJogo* tela_jogo;                        //4
    TelaVitoria* tela_vitoria = new TelaVitoria();  //5
    TelaDerrota* tela_derrota = new TelaDerrota();  //6
    TelaDisconnect* tela_disconnect = new TelaDisconnect(); //7


    // Rodar o codigo enquanto a janela estiver aberta (game loop)
    while(window.isOpen()) {

        // Verificar todos o eventos que ocorreram durante o ultimo loop
        sf::Event event;
        while(window.pollEvent(event)) {
            // Verificar o evento de fechar a janela
            if(event.type == sf::Event::Closed) {
                std::cout << "Execucao finalizada" << std::endl;
                window.close();
            }
            // Verificar clique do mouse (botao esquerdo)
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    
                    mouse_clicado = true;
                    Mouse_clique_position.x = event.mouseButton.x;
                    Mouse_clique_position.y = event.mouseButton.y;
                }
            }
        }

        Mouseposition = sf::Mouse::getPosition(window);

        // Desenho de elementos
        // Limpar tela (obrigatorio)
        window.clear(sf::Color(255,255,255));

        //Conecta no servidor
        while(JanelaAtual==3 && !player->conectado)
        {
            player->conectar();
        }

        // Cria thread para aguardar conexão do player 2
        
        if(JanelaAtual==3 && !wait_for_player2)
        {
            aguarda_player2 = player->thread_aguarda_jogador();
            wait_for_player2=true;
        }

        if(player->player2_conectado  && !iniciou_jogo) 
        {
            JanelaAtual = 4; //Jogo
        }       
        
        if(JanelaAtual == 4 && !iniciou_jogo) {
            tela_jogo = new TelaJogo(player->tipo_jogador,player->Nome,player->nome_adversario);
            envia_tabuleiro = player->thread_envia_tabuleiro();
            recebe_tabuleiro = player->thread_recebe_tabuleiro();
            iniciou_jogo = true;
        }
        
        switch(JanelaAtual)
        {
            case 1:
                tela_login->ReactToMouse(Mouseposition);
                window.draw(*tela_login);
                if(mouse_clicado) JanelaAtual = tela_login->mouseclique(Mouse_clique_position);
                break;
            case 2:
                tela_instrucao->ReactToMouse(Mouseposition);
                window.draw(*tela_instrucao);
                if(mouse_clicado) JanelaAtual = tela_instrucao->mouseclique(Mouse_clique_position);
                break;
            case 3:
                tela_aguardo->change_text();
                tela_aguardo->ReactToMouse(Mouseposition);
                window.draw(*tela_aguardo);
                if(mouse_clicado) JanelaAtual = tela_aguardo->mouseclique(Mouse_clique_position);
                //Aguarda Confirmacao
                
                break;
            case 4:
                tela_jogo->set_nome_turno(player->turno);
                if(mouse_clicado) {
                    player->clicado = tela_jogo->mouseclique(Mouse_clique_position, player,player->turno);
                }
                if(player->recebeu)
                {
                    verifica_vitoria_char = tela_jogo->set_tabuleiro(player);
                    player->recebeu = false;
                }
                window.draw(*tela_jogo);
                sleep = verifica_vitoria(verifica_vitoria_char, player);
                if(player->turno >50 && sleep!=5 && sleep !=6)
                {
                    JanelaAtual = 7;
                }
                if(sleep==5 || sleep ==6 )
                {
                   count_sleep++;
                   if(count_sleep==40)
                   {
                       JanelaAtual = sleep;
                   }
                }
                break;
            case 5:
                window.draw(*tela_vitoria);
                break;
            case 6:
                window.draw(*tela_derrota);
                break;
            case 7:  
                window.draw(*tela_disconnect);
                break;

            default : 
                std::cout << "Execucao finalizada" << std::endl;
                window.close();
                break;
        }

        if(mouse_clicado) mouse_clicado = false;

        window.display();
    }
    return 0;
}