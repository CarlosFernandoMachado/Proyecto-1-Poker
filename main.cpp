#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <ncurses.h>
#include "Carta.h"

using std::string;
using std::ifstream;
using std::ofstream;

Carta* crearmazo();
void imprimircartas(Carta*,int*&);
void imprimircombos();
int juego(Carta*,int*,int);
void imprimircartasvacias();
void guardarArchivo(int dinero);
void abrirArchivo(int& dinero);
int* randomizer(Carta*, int*, bool);
int dobleonada(Carta*,int, int, int);
void imprimirdoscartas(Carta*,int,int);

int main(int argc, char*argv[]){
	initscr();
	start_color();
	int dinero = 0,apuesta = 0,opcion,ganancias;
	bool seguir = true;
	char retener,duplicar = 's';
	Carta* deck = crearmazo();
	int* mantener = new int[5];
	addstr("1) Nueva partida\n2) Cargar Partida\n3) Salir\n");
	scanw("%d\n",&opcion);
	if(opcion == 1){
		addstr("Ingrese cuanto dinero quiere tener: ");
		refresh();
		scanw("%d",&dinero);
		if(dinero <= 0){
			addstr("No puede empezar con una cuenta negativa o vacia\nSe le han acreditado $100 a su cuenta\n");
			dinero = 100;
		}
		while(seguir){
			if(dinero == 0){
				addstr("Se ha quedado sin dinero, desea adicionar dinero a su cuenta?");
				scanw("%d\n",&dinero);
				if(dinero == 0){
					addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
					dinero = 100;	
				}
			}
			clear();
			printw("Usted tiene %d de dinero disponible\n",dinero);
			for(int i = 0; i < 5; i++)
				mantener[i] = -1;
			imprimircombos();
			imprimircartasvacias();
			do{
				if(apuesta <= 0 || apuesta > dinero){
					addstr("Como recordatorio:\nTiene que apostar dinero y no puede apostar mas de lo que tiene\n");
				}
				addstr("Cuanto dinero desea apostar? ");
				refresh();
				scanw("%d",&apuesta);
			}while(apuesta > dinero || apuesta <= 0);
			dinero -= apuesta;
			clear();
			mantener = randomizer(deck,mantener,true);
			imprimircartas(deck,mantener);
			addstr("Porfavor conteste las siguientes preguntas con s y n\ns = si, n = no\nEmpezando desde la izquierda\nCualquier otra respuesta sera tomada como un si\n");
			addstr("Quiere mantener la carta 1: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[0] = -1;
			addstr("Quiere mantener la carta 2: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[1] = -1;
			addstr("Quiere mantener la carta 3: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[2] = -1;
			addstr("Quiere mantener la carta 4: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[3] = -1;
			addstr("Quiere mantener la carta 5: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[4] = -1;
			clear();
			mantener = randomizer(deck,mantener,false);
			imprimircartas(deck,mantener);
			addstr("Ganancias: ");
			refresh();
			ganancias = juego(deck,mantener,apuesta);
			printw("%d\n",ganancias);
			duplicar = 's';
			while(ganancias > 0 && (duplicar == 's' || duplicar == 'S')){
				addstr("Reglas para doble o nada\nSe despliegan 5 cartas nuevas boca abajo usted elije una y luego la maquina escoje una\nsi el valor de la carta de la maquina es igual o mayor al suyo usted pierde sus ganancias\nsi el valor de su carta es mayor que el de la maquina usted duplica sus ganancias\nDoble o nada? s/n: ");
				refresh();
				duplicar = getch();
				if(duplicar == 's' || duplicar == 'S'){
					clear();
					imprimircartasvacias();
					mantener = randomizer(deck,mantener,true);
					int cartapersona = 0,cartamaquina = 0;
					do{
						addstr("De izquierda a derecha(0 a 4)\nElija que carta quiere: ");
						scanw("%d",&cartapersona);
					}while(cartapersona < 0 || cartapersona >= 5);
					addstr("Ahora la maquina elijira una carta!!");
					refresh();
					getch();
					srand(time(NULL));
					do{
						cartamaquina = rand() % 4;
					}while(cartamaquina == cartapersona);
					cartapersona = mantener[cartapersona];
					cartamaquina = mantener[cartamaquina];
					clear();
					addstr("1) Carta persona    2) Carta Maquina\n");
					imprimirdoscartas(deck,cartapersona,cartamaquina);
					ganancias = dobleonada(deck,ganancias,cartapersona,cartamaquina);
					addstr("Ganancias total: ");
					printw("%d\n",ganancias);
				}
			}
			dinero += ganancias;
			refresh();
			getch();
			clear();
			addstr("1)guardar partida y salir\n2)guardar partida y seguir\n3)salir sin guardar\n4)seguir jugando\nOpcion: ");
			scanw("%d\n",&opcion);
			if(opcion == 1){
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				guardarArchivo(dinero);
				seguir = false;
			}else if(opcion == 2){
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				guardarArchivo(dinero);
				clear();
			}else if(opcion == 3){
				seguir = false;
			}else{
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				clear();
			}		
		}
	}else if(opcion == 2){
		abrirArchivo(dinero);
		while(seguir){
			if(dinero == 0){
				addstr("Se ha quedado sin dinero, desea adicionar dinero a su cuenta?");
				scanw("%d\n",&dinero);
				if(dinero == 0){
					addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
					refresh();
					getch();
					dinero = 100;	
				}
			}
			printw("Usted tiene %d de dinero disponible\n",dinero);
			for(int i = 0; i < 5; i++)
				mantener[i] = -1;
			imprimircombos();
			imprimircartasvacias();
			do{
				if(apuesta <= 0 || apuesta > dinero){
					addstr("Como recordatorio:\nTiene que apostar dinero y no puede apostar mas de lo que tiene\n");
				}
				addstr("Cuanto dinero desea apostar? ");
				refresh();
				scanw("%d",&apuesta);
			}while(apuesta > dinero || apuesta <= 0);
			dinero -= apuesta;
			clear();
			mantener = randomizer(deck,mantener,true);
			imprimircartas(deck,mantener);
			addstr("Porfavor conteste las siguientes preguntas con s y n\ns = si, n = no\nEmpezando desde la izquierda\nCualquier otra respuesta sera tomada como un si\n");
			addstr("Quiere mantener la carta 1: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[0] = -1;
			addstr("Quiere mantener la carta 2: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[1] = -1;
			addstr("Quiere mantener la carta 3: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[2] = -1;
			addstr("Quiere mantener la carta 4: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[3] = -1;
			addstr("Quiere mantener la carta 5: ");
			retener = getch();
			addstr("\n");
			if(retener == 'n' || retener == 'N')
				mantener[4] = -1;
			clear();
			mantener = randomizer(deck,mantener,false);
			imprimircartas(deck,mantener);
			addstr("Ganancias: ");
			refresh();
			ganancias = juego(deck,mantener,apuesta);
			printw("%d\n",ganancias);
			duplicar = 's';
			while(ganancias > 0 && (duplicar == 's' || duplicar == 'S')){
				addstr("Reglas para doble o nada\nSe despliegan 5 cartas nuevas boca abajo usted elije una y luego la maquina escoje una\nsi el valor de la carta de la maquina es igual o mayor al suyo usted pierde sus ganancias\nsi el valor de su carta es mayor que el de la maquina usted duplica sus ganancias\nDoble o nada? s/n: ");
				refresh();
				duplicar = getch();
				if(duplicar == 's' || duplicar == 'S'){
					clear();
					imprimircartasvacias();
					mantener = randomizer(deck,mantener,true);
					int cartapersona = 0,cartamaquina = 0;
					do{
						addstr("De izquierda a derecha(0 a 4)\nElija que carta quiere: ");
						scanw("%d",&cartapersona);
					}while(cartapersona < 0 || cartapersona >= 5);
					addstr("Ahora la maquina elijira una carta!!");
					refresh();
					getch();
					srand(time(NULL));
					do{
						cartamaquina = rand() % 4;
					}while(cartamaquina == cartapersona);
					cartapersona = mantener[cartapersona];
					cartamaquina = mantener[cartamaquina];
					clear();
					addstr("1) Carta persona    2) Carta Maquina\n");
					imprimirdoscartas(deck,cartapersona,cartamaquina);
					ganancias = dobleonada(deck,ganancias,cartapersona,cartamaquina);
					addstr("Ganancias total: ");
					printw("%d\n",ganancias);
				}
			}
			dinero += ganancias;
			refresh();
			getch();
			clear();
			addstr("1)guardar partida y salir\n2)guardar partida y seguir\n3)salir sin guardar\n4)seguir jugando\nOpcion: ");
			scanw("%d\n",&opcion);
			if(opcion == 1){
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				guardarArchivo(dinero);
				seguir = false;
			}else if(opcion == 2){
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				guardarArchivo(dinero);
				clear();
			}else if(opcion == 3){
				seguir = false;
			}else{
				if(dinero == 0){
					addstr("Se ha quedado sin dinero, cuanto dinero desea adicionar a su cuenta? ");
					scanw("%d\n",&dinero);
					if(dinero == 0){
						addstr("A nosotros nos importa mucho su diversion por esta razon se le han regalado $100");
						refresh();
						getch();
						dinero = 100;	
					}
				}
				clear();
			}		
		}
	}
	delete[] deck;
	delete[] mantener;
	endwin();
	return 0;
}
int dobleonada(Carta* deck,int dinero, int cartaperson, int cartamachine){
	
	string valorpersona = deck[cartaperson].getValor();	
	string valormaquina = deck[cartamachine].getValor();
	string valores[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
	if(valormaquina == "A"){
		return 0;
	}else if(valorpersona == "A"){
		return (dinero*2);
	}
	for(int i = 12; i > 0; i--){
		if(valormaquina == valores[i]){
			return 0;
		}else if(valorpersona == valores[i]){
			return (dinero*2);
		}
	}
	return 0;
}
void imprimirdoscartas(Carta* deck, int cartapersona, int cartamaquina){
	string color1 = deck[cartapersona].getColor(), color2 = deck[cartamaquina].getColor(), carta1 = deck[cartapersona].getValor(), carta2 = deck[cartamaquina].getValor(), palo1 = deck[cartapersona].getFigura(), palo2 = deck[cartamaquina].getFigura();
	init_pair(1,COLOR_RED,COLOR_WHITE);
	init_pair(2,COLOR_BLACK,COLOR_WHITE);
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  \n");
	//color 1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*");
	if(carta1 == "A"){
		addstr("A");
	}else if(carta1 == "2"){
		addstr("2");
	}else if(carta1 == "3"){
		addstr("3");
	}else if(carta1 == "4"){
		addstr("4");
	}else if(carta1 == "5"){
		addstr("5");
	}else if(carta1 == "6"){
		addstr("6");
	}else if(carta1 == "7"){
		addstr("7");
	}else if(carta1 == "8"){
		addstr("8");
	}else if(carta1 == "9"){
		addstr("9");
	}else if(carta1 == "10"){
		addstr("10");
	}else if(carta1 == "J"){
		addstr("J");
	}else if(carta1 == "Q"){
		addstr("Q");
	}else if(carta1 == "K"){
		addstr("K");
	}
	if(carta1 != "10")
		addstr("            *");
	else
		addstr("           *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *");
	if(carta2 == "A"){
		addstr("A");
	}else if(carta2 == "2"){
		addstr("2");
	}else if(carta2 == "3"){
		addstr("3");
	}else if(carta2 == "4"){
		addstr("4");
	}else if(carta2 == "5"){
		addstr("5");
	}else if(carta2 == "6"){
		addstr("6");
	}else if(carta2 == "7"){
		addstr("7");
	}else if(carta2 == "8"){
		addstr("8");
	}else if(carta2 == "9"){
		addstr("9");
	}else if(carta2 == "10"){
		addstr("10");
	}else if(carta2 == "J"){
		addstr("J");
	}else if(carta2 == "Q"){
		addstr("Q");
	}else if(carta2 == "K"){
		addstr("K");
	}
	if(carta2 != "10")
		addstr("            *");
	else
		addstr("           *");
	addstr(" \n");
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *             * \n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*      ");
	if(palo1 == "♦"){
		addstr("D");
	}else if(palo1 == "♥"){
		addstr("C");
	}else if(palo1 == "♣"){
		addstr("T");
	}else if(palo1 == "♠"){
		addstr("P");	
	}
	addstr("      *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *      ");
	if(palo2 == "♦"){
		addstr("D");
	}else if(palo2 == "♥"){
		addstr("C");
	}else if(palo2 == "♣"){
		addstr("T");
	}else if(palo2 == "♠"){
		addstr("P");	
	}
	addstr("      * \n");
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *  \n");
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta1 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta1 == "A"){
		addstr("A");
	}else if(carta1 == "2"){
		addstr("2");
	}else if(carta1 == "3"){
		addstr("3");
	}else if(carta1 == "4"){
		addstr("4");
	}else if(carta1 == "5"){
		addstr("5");
	}else if(carta1 == "6"){
		addstr("6");
	}else if(carta1 == "7"){
		addstr("7");
	}else if(carta1 == "8"){
		addstr("8");
	}else if(carta1 == "9"){
		addstr("9");
	}else if(carta1 == "10"){
		addstr("10");
	}else if(carta1 == "J"){
		addstr("J");
	}else if(carta1 == "Q"){
		addstr("Q");
	}else if(carta1 == "K"){
		addstr("K");
	}
	addstr("*  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta2 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta2 == "A"){
		addstr("A");
	}else if(carta2 == "2"){
		addstr("2");
	}else if(carta2 == "3"){
		addstr("3");
	}else if(carta2 == "4"){
		addstr("4");
	}else if(carta2 == "5"){
		addstr("5");
	}else if(carta2 == "6"){
		addstr("6");
	}else if(carta2 == "7"){
		addstr("7");
	}else if(carta2 == "8"){
		addstr("8");
	}else if(carta2 == "9"){
		addstr("9");
	}else if(carta2 == "10"){
		addstr("10");
	}else if(carta2 == "J"){
		addstr("J");
	}else if(carta2 == "Q"){
		addstr("Q");
	}else if(carta2 == "K"){
		addstr("K");
	}
	addstr("*  \n");
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  \n");
	if(color1 == "negro"){
		attroff(COLOR_PAIR(2));
	}else{
		attroff(COLOR_PAIR(1));
	}
	refresh();
	getch();
}
void guardarArchivo(int dinero){
	FILE *guardarpartida;
	guardarpartida = fopen("Partida.bin","w");
	fwrite(&dinero,sizeof(int),1,guardarpartida);
	fclose(guardarpartida);
}
void abrirArchivo(int& dinero){
	FILE  *leerpartida;
	leerpartida = fopen("Partida.bin","r");
	fread(&dinero,sizeof(int),1,leerpartida);
	fclose(leerpartida);
}
void imprimircartasvacias(){
	addstr("***************  ***************  ***************  *************** ***************\n");
	addstr("*             *  *             *  *             *  *             * *             *\n");
	addstr("*             *  *             *  *             *  *             * *             *\n");
	addstr("*             *  *             *  *             *  *             * *             *\n");
	addstr("*             *  *             *  *             *  *             * *             *\n");
	addstr("*             *  *             *  *             *  *             * *             *\n");
	addstr("***************  ***************  ***************  *************** ***************\n");
	refresh();
}
int juego(Carta* deck, int* mantener, int dinero){
	string valor1 = deck[mantener[0]].getValor(), valor2 = deck[mantener[1]].getValor(), valor3 = deck[mantener[2]].getValor(), valor4 = deck[mantener[3]].getValor(), valor5 = deck[mantener[4]].getValor(), palo1 = deck[mantener[0]].getFigura(), palo2 = deck[mantener[1]].getFigura(), palo3 = deck[mantener[2]].getFigura(), palo4 = deck[mantener[3]].getFigura(), palo5 = deck[mantener[4]].getFigura(), color1 = deck[mantener[0]].getColor(), color2 = deck[mantener[1]].getColor(), color3 = deck[mantener[2]].getColor(), color4 = deck[mantener[3]].getColor(), color5 = deck[mantener[4]].getColor();
	string valores[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
	int contpar1 = 0, contpar2 = 0;
	//escalera real
	if((palo1 == palo2 && palo2 == palo3 && palo3 == palo4 && palo4 == palo5) && (color1 == color2 && color2 == color3 && color3 == color4 && color4 == color5)){
		if((valor1 == "A" || valor2 == "A" || valor3 == "A" || valor4 == "A" || valor5 == "A") && (valor1 == "10" || valor2 == "10" || valor3 == "10" || valor4 == "10" || valor5 == "10") && (valor1 == "J" || valor2 == "J" || valor3 == "J" || valor4 == "J" || valor5 == "J") && (valor1 == "Q" || valor2 == "Q" || valor3 == "Q" || valor4 == "Q" || valor5 == "Q") && (valor1 == "K" || valor2 == "K" || valor3 == "K" || valor4 == "K" || valor5 == "K")){
			return (dinero*250);
		}
	}
	//ESCALERA + MANJAR-----------------------------------------------------------------------
	for(int i = 0; i < 9; i++){
		if(palo1 == palo2 && palo2 == palo3 && palo3 == palo4 && palo4 == palo5){
			if(valor1 == valores[i] || valor2 == valores[i] || valor3 == valores[i] || valor4 == valores[i] || valor5 == valores[i]){
				if(valor1 == valores[i+1] || valor2 == valores[i+1] || valor3 == valores[i+1] || valor4 == valores[i+1] || valor5 == valores[i+1]){
					if(valor1 == valores[i+2] || valor2 == valores[i+2] || valor3 == valores[i+2] || valor4 == valores[i+2] || valor5 == valores[i+2]){
						if(valor1 == valores[i+3] || valor2 == valores[i+3] || valor3 == valores[i+3] || valor4 == valores[i+3] || valor5 == valores[i+3]){
							if(valor1 == valores[i+4] || valor2 == valores[i+4] || valor3 == valores[i+4] || valor4 == valores[i+4] || valor5 == valores[i+4]){
								return(dinero*50);
							}
						}
					}
				}	
			}
		}		
	}
	//CUATRO IGUALES	
	for(int i = 0; i < 13; i++){
		contpar1 = 0;
		if(valor1 == valores[i])
			contpar1++;
		if(valor2 == valores[i])
			contpar1++;
		if(valor3 == valores[i])
			contpar1++;
		if(valor4 == valores[i])
			contpar1++;
		if(valor5 == valores[i])
			contpar1++;
		if(contpar1 == 4)
			return (dinero*25);
	}
	//FULL HOUSE
	for(int i = 0; i < 13; i++){
		if(contpar1 == 0){
			if(valor1 == valores[i])
				contpar1++;
			if(valor2 == valores[i])
				contpar1++;
			if(valor3 == valores[i])
				contpar1++;
			if(valor4 == valores[i])
				contpar1++;
			if(valor5 == valores[i])
				contpar1++;
			if(contpar1 < 2)
				contpar1 = 0;
		}else if(contpar1 >= 2){
			contpar2 = 0;
			if(valor1 == valores[i])
				contpar2++;
			if(valor2 == valores[i])
				contpar2++;
			if(valor3 == valores[i])
				contpar2++;
			if(valor4 == valores[i])
				contpar2++;
			if(valor5 == valores[i])
				contpar2++;
			if((contpar1 == 3 && contpar2 == 2) || (contpar2 == 3 && contpar1 == 2)){
				return (dinero*9);		
			}
		}
	}
	//MISMO MANJAR------------------------------------------------------------------------------
	if(palo1 == palo2 && palo2 == palo3 && palo3 == palo4 && palo4 == palo5){
		return(dinero*5);
	}
	//escalera--------------------------------------------------------------------------------
	for(int i = 0; i < 10; i++){
		if(valor1 == valores[i] || valor2 == valores[i] || valor3 == valores[i] || valor4 == valores[i] || valor5 == valores[i]){
			if(valor1 == valores[i+1] || valor2 == valores[i+1] || valor3 == valores[i+1] || valor4 == valores[i+1] || valor5 == valores[i+1]){
				if(valor1 == valores[i+2] || valor2 == valores[i+2] || valor3 == valores[i+2] || valor4 == valores[i+2] || valor5 == valores[i+2]){
					if(valor1 == valores[i+3] || valor2 == valores[i+3] || valor3 == valores[i+3] || valor4 == valores[i+3] || valor5 == valores[i+3]){
						if(valor1 == valores[i+4] || valor2 == valores[i+4] || valor3 == valores[i+4] || valor4 == valores[i+4] || valor5 == valores[i+4]){
							return(dinero*4);
						}
					}
				}
			}
		}
	}
	//TRIO
	for(int i = 0; i < 13; i++){
		contpar1 = 0;
		if(valor1 == valores[i])
			contpar1++;
		if(valor2 == valores[i])
			contpar1++;
		if(valor3 == valores[i])
			contpar1++;
		if(valor4 == valores[i])
			contpar1++;
		if(valor5 == valores[i])
			contpar1++;
		if(contpar1 == 3)
			return (dinero*3);
	}
	//DOS PARES,TRIO
	for(int i = 0; i < 13; i++){
		if(contpar1 == 0){
			if(valor1 == valores[i])
				contpar1++;
			if(valor2 == valores[i])
				contpar1++;
			if(valor3 == valores[i])
				contpar1++;
			if(valor4 == valores[i])
				contpar1++;
			if(valor5 == valores[i])
				contpar1++;
			if(contpar1 < 2)
				contpar1 = 0;
		}else if(contpar1 >= 2){
			contpar2 = 0;
			if(valor1 == valores[i])
				contpar2++;
			if(valor2 == valores[i])
				contpar2++;
			if(valor3 == valores[i])
				contpar2++;
			if(valor4 == valores[i])
				contpar2++;
			if(valor5 == valores[i])
				contpar2++;
			if(contpar1 == 2 && contpar2 == 2){
				return (dinero*2);
			}
		}
	}
	//PAR JACKS--------------------------------------------------------------------------------
	for(int i = 0; i < 13; i++){
		contpar1 = 0;
		if(valor1 == valores[i])
			contpar1++;
		if(valor2 == valores[i])
			contpar1++;
		if(valor3 == valores[i])
			contpar1++;
		if(valor4 == valores[i])
			contpar1++;
		if(valor5 == valores[i])
			contpar1++;
		if(contpar1 == 2){
			return dinero;
		}
		if(i == 0){
			i += 9;
		}
	}
	return 0;
}
void imprimircombos(){
	addstr("Par Jack o mejor *1\nDos Pares *2\nTrio *3\nEscalera *4\nMismo Manjar *5\nFull House *9\nCuatro Iguales *25\nEscalera + Manjar *50\nEscalera Real *250\n");
	addstr("P == Pica\tT == Trebol\tD == Diamante\tC == Corazon\n");
	refresh();
}
Carta* crearmazo(){
	Carta* deck = new Carta[52];
	string valores[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
	string color = "negro";
	string palo = "♠";
	int cont = 0;
	for(int i = 0; i <= 51; i++){
		if(i == 26){
			color = "rojo";
		}
		if(i == 13){
			palo = "♣";
		}
		if(i == 26){
			palo = "♥";
		}		
		if(i == 39){
			palo = "♦";
		}		
		if(cont == 13){
			cont = 0;
		}		
		Carta temp(color,palo,valores[cont]);
		deck[i] = temp;
		cont++;
	}
	return deck;
}
int* randomizer(Carta* deck, int* mantener, bool primeravez){
	int random = 0;
	srand(time(NULL));
	if(primeravez){
		random = rand() % 52;
		mantener[0] = random;
		do{
			random = rand() % 52;
			mantener[1] = random;
		}while(random == mantener[0]);
		do{
			random = rand() % 52;
			mantener[2] = random;
		}while(random == mantener[0] || random == mantener[1]);
		do{
			random = rand() % 52;
			mantener[3] = random;
		}while(random == mantener[0] || random == mantener[1] || random == mantener[2]);
		do{
			random = rand() % 52;
			mantener[4] = random;
		}while(random == mantener[0] || random == mantener[1] || random == mantener[2] || random == mantener[3]);
	}else{
		if(mantener[0] == -1){
			do{
				random = rand() % 52;
				mantener[0] = random;
			}while(random == mantener[1] || random == mantener[2] || random == mantener[3] || random == mantener[4]);
		}
		if(mantener[1] == -1){
			do{
				random = rand() % 52;
				mantener[1] = random;
			}while(random == mantener[0] || random == mantener[2] || random == mantener[3] || random == mantener[4]);
		}
		if(mantener[2] == -1){
			do{
				random = rand() % 52;
				mantener[2] = random;
			}while(random == mantener[0] || random == mantener[1] || random == mantener[3] || random == mantener[4]);
		}
		if(mantener[3] == -1){
			do{
				random = rand() % 52;
				mantener[3] = random;
			}while(random == mantener[0] || random == mantener[2] || random == mantener[1] || random == mantener[4]);
		}
		if(mantener[4] == -1){
			do{
				random = rand() % 52;
				mantener[4] = random;
			}while(random == mantener[0] || random == mantener[2] || random == mantener[3] || random == mantener[1]);
		}
	}
	return mantener;
}
void imprimircartas(Carta* deck, int*& mantener){
	string carta1 = deck[mantener[0]].getValor(), carta2 = deck[mantener[1]].getValor(), carta3 = deck[mantener[2]].getValor(), carta4 = deck[mantener[3]].getValor(), carta5 = deck[mantener[4]].getValor(), palo1 = deck[mantener[0]].getFigura(), palo2 = deck[mantener[1]].getFigura(), palo3 = deck[mantener[2]].getFigura(), palo4 = deck[mantener[3]].getFigura(), palo5 = deck[mantener[4]].getFigura(), color1 = deck[mantener[0]].getColor(), color2 = deck[mantener[1]].getColor(), color3 = deck[mantener[2]].getColor(), color4 = deck[mantener[3]].getColor(), color5 = deck[mantener[4]].getColor();
		
	init_pair(1,COLOR_RED,COLOR_WHITE);
	init_pair(2,COLOR_BLACK,COLOR_WHITE);
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*************** ");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************\n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*");
	if(carta1 == "A"){
		addstr("A");
	}else if(carta1 == "2"){
		addstr("2");
	}else if(carta1 == "3"){
		addstr("3");
	}else if(carta1 == "4"){
		addstr("4");
	}else if(carta1 == "5"){
		addstr("5");
	}else if(carta1 == "6"){
		addstr("6");
	}else if(carta1 == "7"){
		addstr("7");
	}else if(carta1 == "8"){
		addstr("8");
	}else if(carta1 == "9"){
		addstr("9");
	}else if(carta1 == "10"){
		addstr("10");
	}else if(carta1 == "J"){
		addstr("J");
	}else if(carta1 == "Q"){
		addstr("Q");
	}else if(carta1 == "K"){
		addstr("K");
	}
	if(carta1 != "10")
		addstr("            *");
	else
		addstr("           *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *");
	if(carta2 == "A"){
		addstr("A");
	}else if(carta2 == "2"){
		addstr("2");
	}else if(carta2 == "3"){
		addstr("3");
	}else if(carta2 == "4"){
		addstr("4");
	}else if(carta2 == "5"){
		addstr("5");
	}else if(carta2 == "6"){
		addstr("6");
	}else if(carta2 == "7"){
		addstr("7");
	}else if(carta2 == "8"){
		addstr("8");
	}else if(carta2 == "9"){
		addstr("9");
	}else if(carta2 == "10"){
		addstr("10");
	}else if(carta2 == "J"){
		addstr("J");
	}else if(carta2 == "Q"){
		addstr("Q");
	}else if(carta2 == "K"){
		addstr("K");
	}
	if(carta2 != "10")
		addstr("            *");
	else
		addstr("           *");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *");
	if(carta3 == "A"){
		addstr("A");
	}else if(carta3 == "2"){
		addstr("2");
	}else if(carta3 == "3"){
		addstr("3");
	}else if(carta3 == "4"){
		addstr("4");
	}else if(carta3 == "5"){
		addstr("5");
	}else if(carta3 == "6"){
		addstr("6");
	}else if(carta3 == "7"){
		addstr("7");
	}else if(carta3 == "8"){
		addstr("8");
	}else if(carta3 == "9"){
		addstr("9");
	}else if(carta3 == "10"){
		addstr("10");
	}else if(carta3 == "J"){
		addstr("J");
	}else if(carta3 == "Q"){
		addstr("Q");
	}else if(carta3 == "K"){
		addstr("K");
	}
	if(carta3 != "10")
		addstr("            *");
	else
		addstr("           *");		
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *");
	if(carta4 == "A"){
		addstr("A");
	}else if(carta4 == "2"){
		addstr("2");
	}else if(carta4 == "3"){
		addstr("3");
	}else if(carta4 == "4"){
		addstr("4");
	}else if(carta4 == "5"){
		addstr("5");
	}else if(carta4 == "6"){
		addstr("6");
	}else if(carta4 == "7"){
		addstr("7");
	}else if(carta4 == "8"){
		addstr("8");
	}else if(carta4 == "9"){
		addstr("9");
	}else if(carta4 == "10"){
		addstr("10");
	}else if(carta4 == "J"){
		addstr("J");
	}else if(carta4 == "Q"){
		addstr("Q");
	}else if(carta4 == "K"){
		addstr("K");
	}
	if(carta4 != "10")
		addstr("            *");
	else
		addstr("           *");		
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr(" *");
	if(carta5 == "A"){
		addstr("A");
	}else if(carta5 == "2"){
		addstr("2");
	}else if(carta5 == "3"){
		addstr("3");
	}else if(carta5 == "4"){
		addstr("4");
	}else if(carta5 == "5"){
		addstr("5");
	}else if(carta5 == "6"){
		addstr("6");
	}else if(carta5 == "7"){
		addstr("7");
	}else if(carta5 == "8"){
		addstr("8");
	}else if(carta5 == "9"){
		addstr("9");
	}else if(carta5 == "10"){
		addstr("10");
	}else if(carta5 == "J"){
		addstr("J");
	}else if(carta5 == "Q"){
		addstr("Q");
	}else if(carta5 == "K"){
		addstr("K");
	}
	if(carta5 != "10")
		addstr("            *\n");
	else
		addstr("           *\n");		
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *             *");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *             *");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *             *");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr(" *             *\n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*      ");
	if(palo1 == "♦"){
		addstr("D");
	}else if(palo1 == "♥"){
		addstr("C");
	}else if(palo1 == "♣"){
		addstr("T");
	}else if(palo1 == "♠"){
		addstr("P");	
	}
	addstr("      *");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *      ");
	if(palo2 == "♦"){
		addstr("D");
	}else if(palo2 == "♥"){
		addstr("C");
	}else if(palo2 == "♣"){
		addstr("T");
	}else if(palo2 == "♠"){
		addstr("P");	
	}
	addstr("      *");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *      ");
	if(palo3 == "♦"){
		addstr("D");
	}else if(palo3 == "♥"){
		addstr("C");
	}else if(palo3 == "♣"){
		addstr("T");
	}else if(palo3 == "♠"){
		addstr("P");	
	}
	addstr("      *");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("  *      ");
	if(palo4 == "♦"){
		addstr("D");
	}else if(palo4 == "♥"){
		addstr("C");
	}else if(palo4 == "♣"){
		addstr("T");
	}else if(palo4 == "♠"){
		addstr("P");	
	}
	addstr("      *");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr(" *      ");
	if(palo5 == "♦"){
		addstr("D");
	}else if(palo5 == "♥"){
		addstr("C");
	}else if(palo5 == "♣"){
		addstr("T");
	}else if(palo5 == "♠"){
		addstr("P");	
	}
	addstr("      *\n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *  ");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *  ");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             * ");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*             *\n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta1 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta1 == "A"){
		addstr("A");
	}else if(carta1 == "2"){
		addstr("2");
	}else if(carta1 == "3"){
		addstr("3");
	}else if(carta1 == "4"){
		addstr("4");
	}else if(carta1 == "5"){
		addstr("5");
	}else if(carta1 == "6"){
		addstr("6");
	}else if(carta1 == "7"){
		addstr("7");
	}else if(carta1 == "8"){
		addstr("8");
	}else if(carta1 == "9"){
		addstr("9");
	}else if(carta1 == "10"){
		addstr("10");
	}else if(carta1 == "J"){
		addstr("J");
	}else if(carta1 == "Q"){
		addstr("Q");
	}else if(carta1 == "K"){
		addstr("K");
	}
	addstr("*  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta2 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta2 == "A"){
		addstr("A");
	}else if(carta2 == "2"){
		addstr("2");
	}else if(carta2 == "3"){
		addstr("3");
	}else if(carta2 == "4"){
		addstr("4");
	}else if(carta2 == "5"){
		addstr("5");
	}else if(carta2 == "6"){
		addstr("6");
	}else if(carta2 == "7"){
		addstr("7");
	}else if(carta2 == "8"){
		addstr("8");
	}else if(carta2 == "9"){
		addstr("9");
	}else if(carta2 == "10"){
		addstr("10");
	}else if(carta2 == "J"){
		addstr("J");
	}else if(carta2 == "Q"){
		addstr("Q");
	}else if(carta2 == "K"){
		addstr("K");
	}
	addstr("*  ");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta3 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta3 == "A"){
		addstr("A");
	}else if(carta3 == "2"){
		addstr("2");
	}else if(carta3 == "3"){
		addstr("3");
	}else if(carta3 == "4"){
		addstr("4");
	}else if(carta3 == "5"){
		addstr("5");
	}else if(carta3 == "6"){
		addstr("6");
	}else if(carta3 == "7"){
		addstr("7");
	}else if(carta3 == "8"){
		addstr("8");
	}else if(carta3 == "9"){
		addstr("9");
	}else if(carta3 == "10"){
		addstr("10");
	}else if(carta3 == "J"){
		addstr("J");
	}else if(carta3 == "Q"){
		addstr("Q");
	}else if(carta3 == "K"){
		addstr("K");
	}
	addstr("*  ");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta4 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta4 == "A"){
		addstr("A");
	}else if(carta4 == "2"){
		addstr("2");
	}else if(carta4 == "3"){
		addstr("3");
	}else if(carta4 == "4"){
		addstr("4");
	}else if(carta4 == "5"){
		addstr("5");
	}else if(carta4 == "6"){
		addstr("6");
	}else if(carta4 == "7"){
		addstr("7");
	}else if(carta4 == "8"){
		addstr("8");
	}else if(carta4 == "9"){
		addstr("9");
	}else if(carta4 == "10"){
		addstr("10");
	}else if(carta4 == "J"){
		addstr("J");
	}else if(carta4 == "Q"){
		addstr("Q");
	}else if(carta4 == "K"){
		addstr("K");
	}
	addstr("* ");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	if(carta5 != "10")
		addstr("*            ");
	else
		addstr("*           ");
	if(carta5 == "A"){
		addstr("A");
	}else if(carta5 == "2"){
		addstr("2");
	}else if(carta5 == "3"){
		addstr("3");
	}else if(carta5 == "4"){
		addstr("4");
	}else if(carta5 == "5"){
		addstr("5");
	}else if(carta5 == "6"){
		addstr("6");
	}else if(carta5 == "7"){
		addstr("7");
	}else if(carta5 == "8"){
		addstr("8");
	}else if(carta5 == "9"){
		addstr("9");
	}else if(carta5 == "10"){
		addstr("10");
	}else if(carta5 == "J"){
		addstr("J");
	}else if(carta5 == "Q"){
		addstr("Q");
	}else if(carta5 == "K"){
		addstr("K");
	}
	addstr("*\n");	
	//color1
	if(color1 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color2
	if(color2 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color3
	if(color3 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************  ");	
	//color4
	if(color4 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("*************** ");	
	//color5
	if(color5 == "negro"){
		attron(COLOR_PAIR(2));
	}else{
		attron(COLOR_PAIR(1));
	}
	addstr("***************\n");
	//blanco
	if(color5 == "negro"){
		attroff(COLOR_PAIR(2));
	}else{
		attroff(COLOR_PAIR(1));
	}
	refresh();
}
