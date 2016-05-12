#include "Carta.h"
#include <string>
#include <iostream>
Carta::Carta(string color,string figura,string valor):color(color),figura(figura),valor(valor){}
Carta::Carta(){
	color = "";
	figura = "";
	valor = "";
}
Carta::~Carta(){
}
string Carta::getColor(){
	return color;
}
string Carta::getValor(){
	return valor;
}
string Carta::getFigura(){
	return figura;
}
