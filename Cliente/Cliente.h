#ifndef CLIENTE_H
#define CLIENTE_H
#include "../common/common.h"
#include <thread>

class Cliente {
    private:
        SAIN addr_servidor;
        int socket_cliente;       
    public:
        Cliente(std::string Nome);
        ~Cliente();
        std::string nome_adversario;
        std::string Nome;
        bool isRunning = true;
        bool adversario_desconectou = false;
        char tipo_jogador;
        int turno = 0;
        bool recebeu = false;
        int  clicado = 0;
        char tabuleiro[3][3];
        bool conectado = false;
        bool player2_conectado = false;
        void conectar();
        std::thread thread_aguarda_jogador();
        void aguarda_jogador();
        std::thread thread_recebe_tabuleiro();        
        void recebe_tabuleiro();
        std::thread thread_envia_tabuleiro();        
        void envia_tabuleiro();
        int verifica_vitoria(char vitoria, Cliente*player);
};
#endif