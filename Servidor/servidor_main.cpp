#include "Servidor.h"

int main() {
    char input = ' ';

    // Criando objeto do servidor
    Servidor* servidor = new Servidor();

    while(input != 'N') {
        cout << "Deseja receber jogadores? (S) ou (N)" << endl;
        input = getchar();
        getchar();// Pega \n
        
        if(input == 'S') {
            servidor->run();
        }
    }

    delete servidor;

    return 0;
}