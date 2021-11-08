# CC = g++
# CFLAGS = -Wall -g
 
# # ****************************************************
# # Targets needed to bring the executable up to date
 
# main: main.o TelaLogin.o 
#     $(CC) $(CFLAGS) -o main main.o TelaLogin.o
 
# # The main.o target can be written more simply
 
# main.o: main.cpp Point.h Square.h
#     $(CC) $(CFLAGS) -c main.cpp
 
# Point.o: Point.h
 
# Square.o: Square.h Point.h



CC = g++

CFLAGS =-g -WAll


all: jogo_da_velha

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

jogo_da_velha: main.o TelaLogin.o TelaAguardo.o TelaDisconnect.o TelaDerrota.o TelaVitoria.o TelaJogo.o TelaInstrucao.o Cliente.o common.o
	g++ main.o TelaLogin.o TelaAguardo.o TelaDisconnect.o TelaDerrota.o TelaVitoria.o TelaJogo.o TelaInstrucao.o Cliente.o common.o -Wall -o jogo_da_velha -lsfml-graphics -lsfml-window -lsfml-system -pthread

clean:
	rm *.o jogo_da_velha

run: jogo_da_velha
	./jogo_da_velha

