#include "common.h"

// Funcao para verificar se uma funcao retornou um erro (-1)
int check (int exp, const char* msg) {
    if(exp < 0) {
        perror(msg);
        exit(1);
    }
    return exp;
}

// Funcao para imprimir a matriz do jogo
void printMatriz3x3(char m[3][3]) {
    int i, j;
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}