#include "TabulkaUzemnychJednotiek.h"

TabulkaUzemnychJednotiek::TabulkaUzemnychJednotiek()
{
	this->pomocna = new ImplicitSequence<UzemnaJednotka*>;
	this->tabulkaTreap = new Treap<string, UzemnaJednotka*>;
	
}

TabulkaUzemnychJednotiek::~TabulkaUzemnychJednotiek()
{
	this->pomocna->clear();
	delete this->pomocna;
	this->tabulkaTreap->clear();
	delete this->tabulkaTreap;
}

Treap<string, UzemnaJednotka*>& TabulkaUzemnychJednotiek::nacitajTreap(ImplicitSequence<UzemnaJednotka>& sekvencia)
{
	auto itZac = sekvencia.begin();
	auto itKon = sekvencia.end();
	while (itZac != itKon)
	{
		string key = itZac.operator*().getOfficialTitle();
		this->tabulkaTreap->insertUnsafe(key, &*itZac);
		++itZac;
	}
	return *tabulkaTreap;
}

ImplicitSequence<UzemnaJednotka*>& TabulkaUzemnychJednotiek::najdiPodlaKlucaVTreap(
	Treap<string, UzemnaJednotka*>& tabulka, string& kluc)
{
	this->pomocna->clear();
	try {
		while (true) {
			UzemnaJednotka* ujPtr = tabulka.removeUnsafe(kluc);
			UzemnaJednotka uj = *ujPtr;

			if (pomocna->isEmpty()) {
				pomocna->insertFirst().data_ = &*ujPtr;
			}
			else {
				pomocna->insertLast().data_ = &*ujPtr;
			}
		}
	}
	catch (std::runtime_error&) {

	}
	return *pomocna;
}
