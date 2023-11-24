#pragma once
#include "UzemnaJednotka.h"
#include "libds/amt/implicit_sequence.h"

using namespace ds::amt;
class SekvenciaUzemnychJednotiek
{
private:
	ImplicitSequence<UzemnaJednotka*>* sekvencia;

public:
	SekvenciaUzemnychJednotiek();
	~SekvenciaUzemnychJednotiek();

	ImplicitSequence<UzemnaJednotka*>& nacitajSekvenciu(ImplicitSequence<UzemnaJednotka>& nacitanaSekvenciaUJ);
};

