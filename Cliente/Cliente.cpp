#include "Cliente.h"

Cliente::Cliente(std::string Nome) {
    // Atribui nome escolhido a classe jogador
    this->Nome = Nome; 
    int i,j;

    //Inicia sem nenhum lugar marcado
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            this->tabuleiro[i][j] = '-';
        }
    }

    // Criado socket
    this->socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    check(this->socket_cliente, "Erro ao criar o socket!");

    // Configurando endereço do servidor
    this->addr_servidor.sin_family = AF_INET;
    this->addr_servidor.sin_port = htons(1234);
    this->addr_servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&addr_servidor.sin_zero, 0, sizeof(addr_servidor.sin_zero));
}

Cliente::~Cliente() {
    //Fecha o socket
    close(this->socket_cliente);
    cout << "Cliente finalizado" << endl;
}

void Cliente::conectar() {
    int i,temp;
    // Estabelecendo conexão com servidor
    cout << "Tentando se conectar ao servidor..." << endl; 

    check(connect(this->socket_cliente, (SA *)&(this->addr_servidor), sizeof(addr_servidor)), 
        "Erro ao se conectar!"
    );

    cout << "Conectado ao servidor!" << endl;

    //Envia o nome para o servidor
    for(i=0; i<256; i++) 
    {   
        temp = htons(this->Nome[i]);
        check(send(socket_cliente, &temp, sizeof(temp), 0),
            "Falha no envio da mensagem ao cliente"
        ); 

        if(this->Nome[i] == '\0') {
            break;
        }
    }
    //Defini que conseguiu conectar
    this->conectado = true; 
}

std::thread Cliente::thread_aguarda_jogador() {
    return std::thread(&Cliente::aguarda_jogador, this);
}

// Função para thread que aguarda o segundo player conectar
void Cliente::aguarda_jogador() {
    // Receber o nome do outro jogador
    char nome_adversario[256] = "";
    check(recv(this->socket_cliente, nome_adversario, sizeof(nome_adversario), 0), 
        "Erro ao receber mensagem"
    );
    
    this->player2_conectado = true;
    this->nome_adversario = nome_adversario;

    // Receber o tipo de caractere que o jogador vai usar, X ou O
    char tipo_jogador;
    check(recv(this->socket_cliente, &tipo_jogador, sizeof(tipo_jogador), 0), 
        "Erro ao receber mensagem"
    );

    this->tipo_jogador = tipo_jogador;

    cout << "Finalizou thread aguarda_jogador" << endl;
}

std::thread Cliente::thread_recebe_tabuleiro() {
    return std::thread(&Cliente::recebe_tabuleiro, this);
}

// Função para thread que recebe atuallizações do tabuleiro do servidor 
void Cliente::recebe_tabuleiro() {
    int i, j, temp;

    while(this->isRunning) {
        //Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(socket_cliente, &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem"
                );
                this->tabuleiro[i][j] = ntohs(temp);
            }
        }

        // Servidor avisa para o cliente finalizar

        // Caso receba matriz de Erro --> Adversário se desconectou
        if(this->tabuleiro[0][0] == 'E') {
            this->isRunning = false; // Para o programa
            this->adversario_desconectou = true;
            break;
        }
        
        this->turno++;
        this->recebeu = true; //Recebeu atualizacao
    }

    cout << "Finalizou thread recebe_tabuleiro" << endl;
}

std::thread Cliente::thread_envia_tabuleiro() {
    return std::thread(&Cliente::envia_tabuleiro, this);
}

// Função para thread que envia atuallizações do tabuleiro para o servidor 
void Cliente::envia_tabuleiro() {
    int i, j, temp;
    
    while(this->isRunning)
    {
        // Só envia atualizações se houve um clique válido na tela (clicado != 0)
        while(this->clicado == 0 && this->isRunning);
        
        // Caso tenha recebido um erro, envia uma matriz confirmando recebimento
        if(this->tabuleiro[0][0] == 'E')
        {
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    temp = htons('D');
                    check(send(socket_cliente, &temp, sizeof(temp), 0),
                        "Falha no envio da mensagem ao cliente"
                    );
                }
            }
            break; 
        }
        
        // Envia atualização se o cliente estiver rodando
        if(this->isRunning)
        {
            //Enviado tabuleiro
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    temp = htons(this->tabuleiro[i][j]);
                    check(send(socket_cliente, &temp, sizeof(temp), 0),
                        "Falha no envio da mensagem ao cliente"
                    );
                }
            }
            // Reinicia indicativo de clique como não clicado (igual a 0)
            this->clicado = 0;
        }
        else
        {
            // Programa cliente finalizado
            //Envia matriz de Erro
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    temp = htons('E');
                    check(send(socket_cliente, &temp, sizeof(temp), 0),
                        "Falha no envio da mensagem ao cliente"
                    );
                }
            }

            break;  //Finaliza thread              
        }        
    }

    cout << "Finalizou thread envia_tabuleiro" << endl;
}

//Verifica vencedor e retorna a janela de vitoria ou derrota
int Cliente::verifica_vitoria(char vitoria, Cliente*player)
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
