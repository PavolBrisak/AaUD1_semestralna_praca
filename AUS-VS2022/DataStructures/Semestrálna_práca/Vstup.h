#pragma once
#include <fstream>

#include "UzemnaJednotka.h"
#include "libds/amt/implicit_sequence.h"
using namespace ds::amt;
class Vstup
{
private:
	ImplicitSequence<UzemnaJednotka>* sekvencia;
	fstream file;
public:
	Vstup();
	~Vstup();
	ImplicitSequence<UzemnaJednotka>& nacitajUdaje(string nazovSuboru);
};

