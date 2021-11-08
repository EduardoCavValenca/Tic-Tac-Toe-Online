CC = g++

CFLAGS =-g -Wall
GAMEFLAGS =  -Wall -lsfml-graphics -lsfml-window -lsfml-system -pthread

all: jogo_da_velha servidor

TelaAguardo.o: telas/TelaAguardo/TelaAguardo.cpp
	$(CC) $(CFLAGS) -c $^

TelaDerrota.o: telas/TelaDerrota/TelaDerrota.cpp
	$(CC) $(CFLAGS) -c $^

TelaDisconnect.o: telas/TelaDisconnect/TelaDisconnect.cpp
	$(CC) $(CFLAGS) -c $^

TelaInstrucao.o: telas/TelaInstrucao/TelaInstrucao.cpp
	$(CC) $(CFLAGS) -c $^

TelaJogo.o: telas/TelaJogo/TelaJogo.cpp
	$(CC) $(CFLAGS) -c $^

TelaLogin.o: telas/TelaLogin/TelaLogin.cpp
	$(CC) $(CFLAGS) -c $^

TelaVitoria.o: telas/TelaVitoria/TelaVitoria.cpp
	$(CC) $(CFLAGS) -c $^

common.o: common/common.cpp
	$(CC) $(CFLAGS) -c $^

Cliente.o: Cliente/Cliente.cpp
	$(CC) $(CFLAGS) -c $^

main.o: main.cpp
	$(CC) $(CFLAGS) -c $^

jogo_da_velha: main.o TelaLogin.o TelaAguardo.o TelaDisconnect.o TelaDerrota.o TelaVitoria.o TelaJogo.o TelaInstrucao.o Cliente.o common.o
	$(CC) main.o TelaLogin.o TelaAguardo.o TelaDisconnect.o TelaDerrota.o TelaVitoria.o TelaJogo.o TelaInstrucao.o Cliente.o common.o $(GAMEFLAGS) -o jogo_da_velha 

servidor: Servidor/Servidor.cpp common.o
	$(CC) Servidor/Servidor.cpp common.o -Wall -pthread -o servidor

clean:
	rm *.o jogo_da_velha servidor

run: jogo_da_velha
	./jogo_da_velha
