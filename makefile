main: main.o Carta.o
	g++ main.o Carta.o -o a 
	
main.o:	main.cpp Carta.h
	g++ -c main.cpp 

Carta.o: Carta.h Carta.cpp
	g++ -c Carta.cpp
