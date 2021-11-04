all: main.o jogo_da_velha

main.o: main.cpp
	g++ -c -Wall main.cpp

jogo_da_velha: main.o
	g++ main.o -Wall -o jogo_da_velha -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o jogo_da_velha

run: jogo_da_velha
	./jogo_da_velha