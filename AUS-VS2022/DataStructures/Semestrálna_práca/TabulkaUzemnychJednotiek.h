#pragma once
#include "UzemnaJednotka.h"
#include "libds/adt/table.h"
using namespace ds::adt;
using namespace ds::amt;

class TabulkaUzemnychJednotiek
{
private:
	ImplicitSequence<UzemnaJednotka*>* pomocna;
	Treap<string, UzemnaJednotka*>* tabulkaTreap;
public:
	TabulkaUzemnychJednotiek();
	~TabulkaUzemnychJednotiek();
	Treap<string, UzemnaJednotka*>& nacitajTreap(ImplicitSequence<UzemnaJednotka>& sekvencia);
	ImplicitSequence<UzemnaJednotka*>& najdiPodlaKlucaVTreap(Treap<string, UzemnaJednotka*>& tabulka, string& kluc);
};

