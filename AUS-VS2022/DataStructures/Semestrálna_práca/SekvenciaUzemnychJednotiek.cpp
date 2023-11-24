#include "SekvenciaUzemnychJednotiek.h"

SekvenciaUzemnychJednotiek::SekvenciaUzemnychJednotiek()
{
	this->sekvencia = new ImplicitSequence<UzemnaJednotka*>;
}

SekvenciaUzemnychJednotiek::~SekvenciaUzemnychJednotiek()
{
	this->sekvencia->clear();
	delete this->sekvencia;
}

ImplicitSequence<UzemnaJednotka*>& SekvenciaUzemnychJednotiek::nacitajSekvenciu(
	ImplicitSequence<UzemnaJednotka>& nacitanaSekvenciaUJ)
{
	auto iteratorBeg = nacitanaSekvenciaUJ.begin();
	auto iteratorEnd = nacitanaSekvenciaUJ.end();
	int index = 0;
	while (iteratorBeg != iteratorEnd)
	{
		this->sekvencia->insert(index).data_ = &*iteratorBeg;
		++index;
		++iteratorBeg;
	}
	return *this->sekvencia;
}
