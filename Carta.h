#pragma once
#include <string>

using std::string;

class Carta{
	string color;
	string figura;
	string valor;
public:
	Carta(string,string,string);
	Carta();
	~Carta();
	string getColor();
	string getValor();
	string getFigura();
};
