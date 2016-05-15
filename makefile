main: main.o Carta.o
	g++ main.o Carta.o -lncurses
	
main.o:	main.cpp Carta.h
	g++ -c main.cpp -lncurses

Carta.o: Carta.h Carta.cpp
	g++ -c Carta.cpp
