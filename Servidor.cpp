#include "Servidor.h"

int socket_player[2];
char nomes[2][256];
char tabuleiro[3][3];
int turno;
bool recebeu;
bool jogou_acabou;
int clienteCount;

int main() {
    pthread_t thread[3];

    // Criando socket
    int socket_servidor;
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    check(socket_servidor, "Erro ao criar socket");
    
    // Configurando endereço do servidor
    SAIN addr_servidor;
    addr_servidor.sin_family = AF_INET;
    addr_servidor.sin_port = htons(1234);
    addr_servidor.sin_addr.s_addr = INADDR_ANY;
    memset(&addr_servidor.sin_zero, 0, sizeof(addr_servidor.sin_zero));

    // Em caso do programa ser interrompido e reiniciado rapidamente
    int one = 1;
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    
    // Fazendo o bind e escutando na port
    check(bind(socket_servidor, (SA *)&addr_servidor, sizeof(addr_servidor)), 
        "Erro na funcao bind()"
    );

    check(listen(socket_servidor, 1), "Erro na funcao listen()");

    // Conectando com 2 players
    clienteCount = 0;
    int i, aux, temp;

   
    while(true) {
        // Iniciando variaveis
        iniciar_variaveis();
        while(clienteCount != 2) {
            cout << "Aguardando player " << clienteCount+1 << endl;

            // Aguardando conexões
            socket_player[clienteCount] = accept(socket_servidor, 0, 0);
            check(socket_player[clienteCount], "Erro na funcao accept()");
            
            // Conexão bem sucedida
            cout << "Cliente " << clienteCount+1 << " conectado!" << endl;

            for(i=0; i<256; i++) {
                check(recv(socket_player[clienteCount], &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem"
                );
                nomes[clienteCount][i] = ntohs(temp);

                if(nomes[clienteCount][i] == '\0') {
                    break;
                }            
            }

            aux = i;

            // Imprime nome
            for(i=0; i<aux; i++) {
                if(nomes[clienteCount][i] == '\0') {
                    break;
                }
                cout << nomes[clienteCount][i];
            }
            cout << endl;

            clienteCount++;
        }

        // Passando o nome de um jogador para o outro jogador
        check(send(socket_player[0], nomes[1], sizeof(nomes[1]), 0),
            "Falha no envio da mensagem ao cliente 1"
        );
        check(send(socket_player[1], nomes[0], sizeof(nomes[0]), 0),
            "Falha no envio da mensagem ao cliente 2"
        );

        // Determinando o tipo de caractere que cada jogador vai usar
        char tipo_jogador1 = 'X';
        char tipo_jogador2 = 'O';
        check(send(socket_player[0], &tipo_jogador1, sizeof(tipo_jogador1), 0),
            "Falha no envio da mensagem ao cliente 3"
        );
        check(send(socket_player[1], &tipo_jogador2, sizeof(tipo_jogador2), 0),
            "Falha no envio da mensagem ao cliente 4"
        );

        /**
        * Criando threads para lidar com a conexão com clientes.
        * Uma fica responsável por enviar mensagens aos clientes, enquanto as outras
        * ficam responsáveis por receber mensagens dos clientes.
        * Note que esses processos ocorrem de forma independente.
        **/

        pthread_create(&thread[0], NULL, envia_tabuleiro, NULL);
        pthread_create(&thread[1], NULL, recebe_tabuleiro_player1, NULL);
        pthread_create(&thread[2], NULL, recebe_tabuleiro_player2, NULL);
        
        // Finalizando threads e socket

        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);
        pthread_join(thread[2], NULL);
        
        close(socket_player[0]);
        close(socket_player[1]);
    }
    close(socket_servidor);
    
    return 0;
}

// Função para enviar o tabuleiro atual para os dois players
void *envia_tabuleiro(void *) {
    int i, j, temp;
    bool vitoriaX=false, vitoriaO=false;

    while(!jogou_acabou) {
        while(!recebeu);

        // Verificando se X venceu
        vitoriaX = verifica_vitoria('X');
        if(vitoriaX) {
            cout << "Vitoria do X" << endl;

            // Se X venceu retorna tabuleiro cheio de X
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    tabuleiro[i][j] = 'X';
                }
            }

            // Fim de jogo
            jogou_acabou = true;
        } 

        // Verificando se O venceu
        vitoriaO = verifica_vitoria('O');
        if(vitoriaO) {
            cout << "Vitoria do O" << endl;

            // Se O venceu retorna tabuleiro cheio de O
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    tabuleiro[i][j] = 'O';
                }
            }

            // Fim de jogo
            jogou_acabou = true;
        } 

        // Verificando empate
        if(!vitoriaX && !vitoriaO && verifica_tabuleiro_cheio()) {
            cout << "Empate" << endl;

            // Se ocorreu empate, limpa tabuleiro
            for(i=0; i<3; i++) {
                for(j=0; j<3; j++) {
                    tabuleiro[i][j] = '-';
                }
            }

        } 

        //Enviando tabuleiro para os players
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                temp = htons(tabuleiro[i][j]);
                check(send(socket_player[0], &temp, sizeof(temp), 0),
                    "Falha no envio da mensagem ao cliente 5"
                );
                check(send(socket_player[1], &temp, sizeof(temp), 0),
                    "Falha no envio da mensagem ao cliente 6"
                );
            }
        }
        recebeu=false;
    }

    return NULL;
}

// Função para receber tabuleiro do player 1
void *recebe_tabuleiro_player1(void *) {
    int i, j, temp;

    while(!jogou_acabou) {
        // Só atualizamos o tabuleiro com base no enviado pelo player1 se
        // o turno pertencer a esse jogador
        // Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(socket_player[0], &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem"
                );
                tabuleiro[i][j] = ntohs(temp);
            }
        }
        // Imprimindo novo tabuleiro
        printMatriz3x3(tabuleiro);
        recebeu = true;
    }

    return NULL;
}

// Função para receber tabuleiro do player 2
void *recebe_tabuleiro_player2(void *) {
    int i, j, temp;

    while(!jogou_acabou) {
        // Só atualizamos o tabuleiro com base no enviado pelo player2 se
        // o turno pertencer a esse jogador
        // if(turno == 2) {
        // Recebendo tabuleiro
        for(i=0; i<3; i++) {
            for(j=0; j<3; j++) {
                check(recv(socket_player[1], &temp, sizeof(temp), 0), 
                    "Erro ao receber mensagem"
                );
                // Atualizando tabuleiro
                tabuleiro[i][j] = ntohs(temp);
            }
        }
        // Imprimindo novo tabuleiro
        printMatriz3x3(tabuleiro);

        recebeu = true;
        // }
    }

    return NULL;
}

bool verifica_vitoria(char tipo) {
    int i, j;
    int count = 0;

    // Verificando colunas
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(tabuleiro[i][j] == tipo) {
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
            if(tabuleiro[j][i] == tipo) {
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
        if(tabuleiro[i][i] == tipo) {
            count++;
        }
    }
    if(count == 3) {
        return true;
    }

    count = 0;
    for(i=0; i<3; i++) {
        if(tabuleiro[2-i][i] == tipo) {
            count++;
        }
    }
    if(count == 3) {
        return true;
    }

    return false;
}

bool verifica_tabuleiro_cheio() {
    int i, j;

    // Verifica se o tabuleiro está cheio
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(tabuleiro[i][j] == '-') {
                return false;
            }
        }
    }

    return true;
}

void iniciar_variaveis() {

        int i,j;
        socket_player[0] = 0;
        socket_player[1] = 0;

        clienteCount = 0;
        jogou_acabou = false;
        nomes[0][0] = '\0';
        nomes[1][0] ='\0';
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                 tabuleiro[i][j] ='-';
            }
        }

        turno = 1;
        recebeu = false;
        jogou_acabou = false;
}