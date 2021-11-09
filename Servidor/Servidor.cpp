#include "Servidor.h"

Servidor::Servidor() {
    // Criando socket
    this->socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    check(this->socket_servidor, "Erro ao criar socket");
    
    // Configurando endereço do servidor
    this->addr_servidor.sin_family = AF_INET;
    this->addr_servidor.sin_port = htons(1234);
    this->addr_servidor.sin_addr.s_addr = INADDR_ANY;
    memset(&(this->addr_servidor).sin_zero, 0, sizeof((this->addr_servidor).sin_zero));

    // Em caso do programa ser interrompido e reiniciado rapidamente
    int one = 1;
    setsockopt(this->socket_servidor, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    
    // Fazendo o bind e escutando na port
    check(bind(this->socket_servidor, (SA *)&(this->addr_servidor), sizeof(this->addr_servidor)), 
        "Erro na funcao bind()"
    );

    check(listen(this->socket_servidor, 2), "Erro na funcao listen()");
}

// Destrutor
Servidor::~Servidor() {
    close(this->socket_servidor);
    close(this->socket_player[0]);
    close(this->socket_player[1]);
    cout << "servidor finalizado" << endl;
}

void Servidor::iniciar_variaveis() {
    int i,j;
    this->socket_player[0] = 0;
    this->socket_player[1] = 0;

    this->jogou_acabou = false;
    this->nomes[0][0] = '\0';
    this-> nomes[1][0] ='\0';
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            this->tabuleiro[i][j] ='-';
        }
    }

    this->turno = 1;
    this->recebeu = false;
    this->jogou_acabou = false;
    this->recebeuErro = false;
}

void Servidor::run() {
    // Conectando com 2 players
    this->iniciar_variaveis();
    this->recebe_conexoes(); //Aguarda players
    
    // Passando o nome de um jogador para o outro jogador
    check(send(this->socket_player[0], this->nomes[1], sizeof(this->nomes[1]), 0),
        "Falha no envio da mensagem ao cliente 1"
    );
    check(send(this->socket_player[1], this->nomes[0], sizeof(this->nomes[0]), 0),
        "Falha no envio da mensagem ao cliente 2"
    );

    // Determinando o tipo de caractere que cada jogador vai usar
    char tipo_jogador1 = 'X';
    char tipo_jogador2 = 'O';
    check(send(this->socket_player[0], &tipo_jogador1, sizeof(tipo_jogador1), 0),
        "Falha no envio da mensagem ao cliente 3"
    );
    check(send(this->socket_player[1], &tipo_jogador2, sizeof(tipo_jogador2), 0),
        "Falha no envio da mensagem ao cliente 4"
    );

    /** 
     * Criando threads para lidar com a conexão com clientes.
     * Uma fica responsável por enviar mensagens aos clientes, enquanto as outras
     * ficam responsáveis por receber mensagens dos clientes.
     * Note que esses processos ocorrem de forma independente.
    **/

    this->thread_envia_tabuleiro = std::thread(&Servidor::envia_tabuleiro, this);
    this->thread_recebe_tabuleiro_player1 = std::thread(&Servidor::recebe_tabuleiro_player1, this);
    this->thread_recebe_tabuleiro_player2 = std::thread(&Servidor::recebe_tabuleiro_player2, this);
    
    // Finalizando threads e sockets
    if(this->thread_envia_tabuleiro.joinable()) {
        this->thread_envia_tabuleiro.join();
    }

    if(this->thread_recebe_tabuleiro_player1.joinable()) {
        this->thread_recebe_tabuleiro_player1.join();
    }

    if(this->thread_recebe_tabuleiro_player2.joinable()) {
        this->thread_recebe_tabuleiro_player2.join();
    }
    //Finalizando conexão
    close(socket_player[0]);
    close(socket_player[1]);
}

bool Servidor::verifica_vitoria(char tipo) {
    int i, j;
    int count = 0;

    // Verificando colunas
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(this->tabuleiro[i][j] == tipo) {
                count++;
            }
        }
        if(count == 3) {
            return true;
        }
        else{
            count = 0;
        }
    }

    // Verificando linhas
    count = 0;
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(this->tabuleiro[j][i] == tipo) {
                count++;
            }
        }  
        if(count == 3) {
            return true;
        }
        else{
            count = 0;
        }
    }

    // Verificando diagonais
    count = 0;
    for(i=0; i<3; i++) {
        if(this->tabuleiro[i][i] == tipo) {
            count++;
        }
    }
    if(count == 3) {
        return true;
    }

    count = 0;
    for(i=0; i<3; i++) {
        if(this->tabuleiro[2-i][i] == tipo) {
            count++;
        }
    }
    if(count == 3) {
        return true;
    }

    return false;
}

bool Servidor::verifica_tabuleiro_cheio() {
    int i, j;

    // Verifica se o tabuleiro está cheio de X ou O
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(this->tabuleiro[i][j] == '-' || this->tabuleiro[i][j] == 'E') {
                return false;
            }
        }
    }

    return true;
}

void Servidor::recebe_conexoes() {
    int i = 0, j, temp;

    // Recebe conexão de dois jogadores
    while(i != 2) {
        cout << "Aguardando player " << i+1 << endl;

        // Aguardando conexões
        this->socket_player[i] = accept(this->socket_servidor, 0, 0);
        check(this->socket_player[i], "Erro na funcao accept()");

        // Conexão bem sucedida
        cout << "Cliente " << i+1 << " conectado!" << endl;

        // Recebe nome dos jogadores
        for(j=0; j<256; j++) {
            check(recv(this->socket_player[i], &temp, sizeof(temp), 0), 
                "Erro ao receber mensagem 1"
            );
            this->nomes[i][j] = ntohs(temp);

            if(this->nomes[i][j] == '\0') {
                break;
            }            
        }
        i++;
    }
}

// Função para enviar o tabuleiro atual para os dois players
void Servidor::envia_tabuleiro() {
    int i, j, temp;
    bool vitoriaX = false, vitoriaO = false;

    while(!this->jogou_acabou) {
        
        while(!this->recebeu);//Aguarda receber para enviar outra atualizacao

        // Recebendo erro deve-se avisar ao outro jogador que um se desconectou
        if(this->recebeuErro) {
            this->avisa_desconexao();
            break;
        }

        // Verificando se X venceu
        vitoriaX = this->verifica_vitoria('X');
        if(vitoriaX) {
            cout << "Vitoria do X" << endl;

            // Se X venceu retorna tabuleiro cheio de X
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    this->tabuleiro[i][j] = 'X';
                }
            }

            // Fim de jogo
            this->jogou_acabou = true;
        } 

        // Verificando se O venceu
        vitoriaO = this->verifica_vitoria('O');
        if(vitoriaO) {
            cout << "Vitoria do O" << endl;

            // Se O venceu retorna tabuleiro cheio de O
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    this->tabuleiro[i][j] = 'O';
                }
            }

            // Fim de jogo
            this->jogou_acabou = true;
        } 

        // Verificando empate
        if(!vitoriaX && !vitoriaO && this->verifica_tabuleiro_cheio()) {
            cout << "Empate" << endl;

            // Se ocorreu empate, limpa tabuleiro
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    this->tabuleiro[i][j] = '-';
                }
            }
        }        

        //Enviando tabuleiro para os players
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                temp = htons(this->tabuleiro[i][j]);
                check(send(this->socket_player[0], &temp, sizeof(temp), 0),
                    "Falha no envio da mensagem ao cliente 5"
                );
                check(send(this->socket_player[1], &temp, sizeof(temp), 0),
                    "Falha no envio da mensagem ao cliente 6"
                );
            }
        }
        this->recebeu=false;
    }
}

// Função para receber tabuleiro do player 1
void Servidor::recebe_tabuleiro_player1() {
    int i, j, temp;

    while(!this->jogou_acabou) {
        // Só atualizamos o tabuleiro com base no enviado pelo player1 se
        // o turno pertencer a esse jogador
        // Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(this->socket_player[0], &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem 2"
                );
                this->tabuleiro[i][j] = ntohs(temp);
            }
        }

        // Imprimindo novo tabuleiro
        printMatriz3x3(this->tabuleiro);   
    
        this->recebeu = true; 

        // Verifica se recebeu erro 
        if(this->tabuleiro[0][0] == 'E') {
            this->recebeuErro=true;
            break;
        }

        // Confirma que o segundo player se desconectou
        if(this->tabuleiro[0][0] == 'D') {
            break;
        }
    }

}

// Função para receber tabuleiro do player 2
void Servidor::recebe_tabuleiro_player2() {
    int i, j, temp;

    while(!this->jogou_acabou) {
        // Só atualizamos o tabuleiro com base no enviado pelo player2 se
        // o turno pertencer a esse jogador
        // if(turno == 2) {
        // Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(this->socket_player[1], &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem 3"
                );
                // Atualizando tabuleiro
                this->tabuleiro[i][j] = ntohs(temp);
            }
        }

        // Imprimindo novo tabuleiro
        printMatriz3x3(this->tabuleiro);

        this->recebeu = true;

        // Verifica se recebeu erro 
        if(this->tabuleiro[0][0] == 'E') {
            cout << "Recebi Erro do 2" <<endl;
            this->recebeuErro=true;
            break;
        }

        //Confirma que o segundo player se desconectou
        if(this->tabuleiro[0][0] == 'D') {
            break;
        }
    }
}

//Avisa aos players que um deles se desconectou
void Servidor::avisa_desconexao() {
    int i, j, temp;

    // Verificando se os sockets são válidos
    if(this->socket_player[0] == 0 && this->socket_player[1] == 0) {
        return;
    }

    // Enviando erro
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            temp = htons('E');
            check(send(this->socket_player[1], &temp, sizeof(temp), 0),
                "Falha no envio da mensagem ao cliente 5"
            );
            check(send(this->socket_player[0], &temp, sizeof(temp), 0),
                "Falha no envio da mensagem ao cliente 5"
            );
        }
    }
}