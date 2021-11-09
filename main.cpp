#include <iostream>
#include <SFML/Graphics.hpp>
#include "telas/TelaLogin/TelaLogin.h"
#include "telas/TelaAguardo/TelaAguardo.h"
#include "telas/TelaDisconnect/TelaDisconnect.h"
#include "telas/TelaDerrota/TelaDerrota.h"
#include "telas/TelaVitoria/TelaVitoria.h"
#include "telas/TelaJogo/TelaJogo.h"
#include "telas/TelaInstrucao/TelaInstrucao.h"
#include "Cliente/Cliente.h"

int main(int argc, char *argv[])
{
    // Criando variáveis uteis para a lógica do jogo
    char verifica_vitoria_char = '-';
    int count_sleep=0;
    char sleep;
    
    // Obtendo nome do jogador
    string Nome;
    std::cout << "Digite seu Nome: " << std::endl;
    std::getline(cin,Nome);

    // Criando objeto responsável pela comunicação do jogo com o servidor
    Cliente* player = new Cliente(Nome);

    // Criando threads e variáveis uteis para manipulação do player/cliente
    std::thread thread_aguarda_player2;
    std::thread thread_recebe_tabuleiro;
    std::thread thread_envia_tabuleiro;
    bool wait_for_player2 = false;
    bool iniciou_jogo = false;

    /** Iniciando lógica da parte gráfica **/

    // Variáveis que guarda qual janela deve ser exibida
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
    if(!icon.loadFromFile("texturas/Icon.png"))
    {
        std::cout << "Erro ao obter ícone" << std::endl;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Configurando variáveis pra controle do clique do mouse
    sf::Vector2i Mouse_clique_position;
    sf::Vector2i Mouseposition;
    bool mouse_clicado = false;

    // Criando objetos para manipulação das telas do jogo
    TelaLogin* tela_login = new TelaLogin(Nome);            //1
    TelaInstrucao* tela_instrucao = new TelaInstrucao();    //2
    TelaAguardo* tela_aguardo = new TelaAguardo();          //3
    TelaJogo* tela_jogo;                                    //4
    TelaVitoria* tela_vitoria = new TelaVitoria();          //5
    TelaDerrota* tela_derrota = new TelaDerrota();          //6
    TelaDisconnect* tela_disconnect = new TelaDisconnect(); //7

    /** Gameloop **/

    // Rodar o codigo enquanto a janela estiver aberta (game loop)
    while(window.isOpen()) 
    {
        // Verificar todos o eventos que ocorreram durante o ultimo loop
        sf::Event event;

        while(window.pollEvent(event)) 
        {
            // Verificar o evento de fechar a janela
            if(event.type == sf::Event::Closed) {
                std::cout << "Execucao finalizada" << std::endl;
                window.close();
                //Faz com que termine as thread de envio e recebimento
                player->isRunning=false; 
            }
            // Verificar clique do mouse (botão esquerdo)
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    mouse_clicado = true;
                    Mouse_clique_position.x = event.mouseButton.x;
                    Mouse_clique_position.y = event.mouseButton.y;
                }
            }
        }
        
        // Garantir que o gameloop acabe ao clicar em fechar
        if(!window.isOpen()) {
            break;
        }

        /** Tratamento dos dados dos eventos **/

        // Posição do mouse
        Mouseposition = sf::Mouse::getPosition(window);

        // Desenho de elementos
        // Limpar tela (obrigatório)
        window.clear(sf::Color(255,255,255));

        //Conecta no servidor
        while(JanelaAtual==3 && !player->conectado)
        {
            player->conectar();
        }

        // Cria thread para aguardar conexão do player 2
        if(JanelaAtual==3 && !wait_for_player2)
        {
            thread_aguarda_player2 = player->thread_aguarda_jogador();
            wait_for_player2=true;
        }

        // Verifica se devemos redirecionar a tela do jogador para a tela de jogo
        if(player->player2_conectado && !iniciou_jogo) 
        {
            JanelaAtual = 4; //Jogo
        }       
        
        // Na primeira vez que o jogador clica para jogar, criamos as threads para comunicação
        // com o servidor
        if(JanelaAtual == 4 && !iniciou_jogo)
        {
            // Criando objeto de tela de jogo
            tela_jogo = new TelaJogo(player->tipo_jogador, player->Nome, player->nome_adversario);

            // Inicialiando threads
            thread_envia_tabuleiro = player->thread_envia_tabuleiro();
            thread_recebe_tabuleiro = player->thread_recebe_tabuleiro();

            iniciou_jogo = true;
        }
        
        //Nao atualiza janela, e finaliza o programa
        if(player->adversario_desconectou) {
            JanelaAtual = 7;
        }

        // Verificando qual janela deve ser desenhada na tela e tratando dados
        switch(JanelaAtual)
        {
            case 1:
                // Comunica que a tela deve monitorar o mouse
                tela_login->ReactToMouse(Mouseposition);

                // Desenha a tela na janela do jogo
                window.draw(*tela_login);

                // Determinando próxima tela com base no clique do mouse
                if(mouse_clicado)
                {
                    JanelaAtual = tela_login->mouseclique(Mouse_clique_position);
                }
                break;
            case 2:
                // Comunica que a tela deve monitorar o mouse
                tela_instrucao->ReactToMouse(Mouseposition);

                // Desenha a tela na janela do jogo
                window.draw(*tela_instrucao);

                // Determinando próxima tela com base no clique do mouse
                if(mouse_clicado)
                {
                    JanelaAtual = tela_instrucao->mouseclique(Mouse_clique_position);
                }
                break;
            case 3:
                // Animação de texto
                tela_aguardo->change_text();

                // Comunica que a tela deve monitorar o mouse
                tela_aguardo->ReactToMouse(Mouseposition);

                // Desenha a tela na janela do jogo
                window.draw(*tela_aguardo);

                // Determinando próxima tela com base no clique do mouse
                if(mouse_clicado) {
                     JanelaAtual = tela_aguardo->mouseclique(Mouse_clique_position);
                }                
                break;
            case 4:
                // Imprimindo nome do jogador do turno atual
                tela_jogo->set_nome_turno(player->turno);
                
                // Monitorando o clique do mouse
                if(mouse_clicado) {
                    player->clicado = tela_jogo->mouseclique(Mouse_clique_position, player,player->turno);
                }

                // Monitorando a atualização do tabuleiro vindo do servidor
                if(player->recebeu)
                {
                    verifica_vitoria_char = tela_jogo->set_tabuleiro(player);
                    player->recebeu = false;
                }

                // Desenha a tela na janela do jogo
                window.draw(*tela_jogo);

                // Verificando vitória e dando sleep na tela
                sleep = player->verifica_vitoria(verifica_vitoria_char, player);

                //Se um dos jogadores se desconecta
                if(player->turno > 100 && sleep!=5 && sleep !=6)
                {
                    JanelaAtual = 7;
                }
                if(sleep==5 || sleep ==6 ) //Se alguem ganhou
                {
                   count_sleep++;
                   if(count_sleep==300) //Delay
                   {
                       JanelaAtual = sleep;
                   }
                }
                break;
            case 5:
                // Desenha a tela na janela do jogo
                window.draw(*tela_vitoria);
                break;
            case 6:
                // Desenha a tela na janela do jogo
                window.draw(*tela_derrota);
                break;
            case 7:  
                // Desenha a tela na janela do jogo
                window.draw(*tela_disconnect);
                break;

            default : 
                std::cout << "Execucao finalizada" << std::endl;
                window.close();
                break;
        }

        if(mouse_clicado) mouse_clicado = false;

        // Exibe a janela
        window.display();
    }

    // Jogo finalizado
    // Indicando às threads recebe_tabuleiro e envia_tabuleiro que podem acabar
    player->isRunning = false;
    
    //Se thread estao ativas, aguarda ate finalizarem
    if(thread_aguarda_player2.joinable()) {
        thread_aguarda_player2.join();
    }
    if(thread_recebe_tabuleiro.joinable()) {
        thread_recebe_tabuleiro.join();
    }
    if(thread_envia_tabuleiro.joinable()) {
        thread_envia_tabuleiro.join();
    }

    // Liberando espaços de memória
    delete player;
    delete tela_login;
    delete tela_instrucao;
    delete tela_aguardo;
    delete tela_jogo;
    delete tela_vitoria;
    delete tela_derrota;
    delete tela_disconnect;

    return 0;
}