#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // Criando configuracoes de contexto para ter formas com melhores contornos
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Criando a janela do jogo
    sf::RenderWindow window(
        sf::VideoMode(800, 600), "Jogo da Velha", sf::Style::Titlebar | sf::Style::Close, settings
    );
    window.setPosition(sf::Vector2i(100, 50));

    // Importando icone da aplicacao
    sf::Image icon;
    if(!icon.loadFromFile("icon.png")) {
        std::cout << "vixi" << std::endl;
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Carregando fonte
    sf::Font font;
    if(!font.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Erro ao carregar fonte" << std::endl;
    }

    // Criando titulo
    sf::Text titulo;
    titulo.setFont(font);
    titulo.setString("Jogo da Velha");
    titulo.setCharacterSize(24); // em px
    titulo.setFillColor(sf::Color::White);
    // Centralizando o texto de titulo
    titulo.setPosition(sf::Vector2f((float) (400.0 - titulo.getLocalBounds().width / 2.0), 5.0));

    // Criando um plano de fundo
    sf::RectangleShape background(sf::Vector2f(800.0, 600.0));
    background.setFillColor(sf::Color(20, 189, 172));
    
    // Carregando textura do grid
    sf::Texture textura;
    if(!textura.loadFromFile("gridBranco.png")) {
        std::cout << "Erro ao carregar textura" << std::endl;
    }

    // Criando grid
    sf::Sprite grid;
    grid.setTexture(textura);
    grid.setPosition(sf::Vector2f(200.0, 100.0));

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
            //Verificar entrada de texto
            if(event.type == sf::Event::TextEntered) {
                // Verificando se é um character ASCII
                if(event.text.unicode < 128) {
                    std::cout << "Caractere ASCII digitado: " << static_cast<char>(event.text.unicode) << std::endl;
                }
            }
            // Verificar clique do mouse (botao esquerdo)
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    std::cout << "Botao esquerdo do mouse acionado" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                }
            }
        }

        // Desenho de elementos
        // Limpar tela (obrigatorio)
        window.clear(sf::Color(20, 189, 172));

        window.draw(background);
        window.draw(grid);
        window.draw(titulo);

        // Fim do frame atual
        window.display();
    }
    return 0;
}