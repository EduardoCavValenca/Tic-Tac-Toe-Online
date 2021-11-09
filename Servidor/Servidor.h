#ifndef SERVIDOR_H
#define SERVIDOR_H
#include <thread>
#include "../common/common.h"

class Servidor {
    private:
        int socket_servidor;
        SAIN addr_servidor;
        int socket_player[2];
        char nomes[2][256];
        char tabuleiro[3][3];
        int turno;
        bool recebeu;
        bool jogou_acabou;
        bool isRunning = true;
        bool recebeuErro = false;
        std::thread thread_envia_tabuleiro;
        std::thread thread_recebe_tabuleiro_player1;
        std::thread thread_recebe_tabuleiro_player2;
        //std::thread thread_finaliza_servidor;
        std::thread thread_listen_player1;
        std::thread thread_listen_player2;
    
    public:
        Servidor();
        ~Servidor();
        void run();
        void iniciar_variaveis();
        bool verifica_vitoria(char tipo);
        bool verifica_tabuleiro_cheio();
        void envia_tabuleiro();
        void recebe_tabuleiro_player1();
        void recebe_tabuleiro_player2();
        //void finaliza_servidor();
        void recebe_conexoes();
        void avisa_desconexao();
};

#endif