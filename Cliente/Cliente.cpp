#include "Cliente.h"

Cliente::Cliente(std::string Nome) {
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
    this->conectado = true; 
}


std::thread Cliente::thread_aguarda_jogador() {
    return std::thread(&Cliente::aguarda_jogador, this);
}

void Cliente::aguarda_jogador() {
    // Receber o nome do outro jogador
    char nome_adversario[256] = "";
    check(recv(this->socket_cliente, nome_adversario, sizeof(nome_adversario), 0), 
        "Erro ao receber mensagem"
    );
    cout << nome_adversario << endl;
    
    this->player2_conectado = true;

    this->nome_adversario = nome_adversario;

    // Receber o tipo de caractere que o jogador vai usar
    char tipo_jogador;
    check(recv(this->socket_cliente, &tipo_jogador, sizeof(tipo_jogador), 0), 
        "Erro ao receber mensagem"
    );

    this->tipo_jogador = tipo_jogador;
}

std::thread Cliente::thread_recebe_tabuleiro() {
    return std::thread(&Cliente::recebe_tabuleiro, this);
}

void Cliente::recebe_tabuleiro() {
    int i, j, temp;

    while(true) {
        //Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(socket_cliente, &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem"
                );
                this->tabuleiro[i][j] = ntohs(temp);
            }
        }

        this->turno++;
        this->recebeu = true;
        // Imprimindo
        printMatriz3x3(this->tabuleiro);
    }
}

std::thread Cliente::thread_envia_tabuleiro() {
    return std::thread(&Cliente::envia_tabuleiro, this);
}

void Cliente::envia_tabuleiro() {
    int i, j, temp;
    
    while(true)
    {
        while(this->clicado == 0);
            //Enviado tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                temp = htons(this->tabuleiro[i][j]);
                check(send(socket_cliente, &temp, sizeof(temp), 0),
                    "Falha no envio da mensagem ao cliente"
                );
            }
        }
        this->clicado = 0;
    }
}    
