#pragma once
#include <functional>

#include "UzemnaJednotka.h"
using namespace ds::amt;

class Algoritmus
{
private:
	ImplicitSequence<UzemnaJednotka*>* pomocna;

public:
	Algoritmus()
	{
		this->pomocna = new ImplicitSequence<UzemnaJednotka*>;
	}
	~Algoritmus()
	{
		this->pomocna->clear();
		delete this->pomocna;
	}

	template<typename Iterator>
	ImplicitSequence<UzemnaJednotka*>& dajDataKtoreSplnajuPredikat(Iterator iteratorBegin, Iterator iteratorEnd, std::function<bool(UzemnaJednotka*)> predicate) const
	{
		this->pomocna->clear();
		int index = 0;
		while (iteratorBegin != iteratorEnd)
		{
			if (predicate(*iteratorBegin))
			{
				pomocna->insert(index).data_ = *iteratorBegin;
				++iteratorBegin;
				++index;
			}
			else {
				++iteratorBegin;
			}
		}
		return *pomocna;
	}

	/*template<typename Iterator>
	ImplicitSequence<UzemnaJednotka*> startsWithStr(Iterator iteratorBegin, Iterator iteratorEnd, string retazec) const
	{
		ImplicitSequence<UzemnaJednotka*> s1 = this->dajDataKtoreSplnajuPredikat<Iterator>(iteratorBegin, iteratorEnd,[&](UzemnaJednotka* a) {

			if (a->getShortTitle().substr(0, retazec.size()) == retazec)
			{
				return true;
			}
			return false;
			});

		return s1;
	}

	template<typename Iterator>
	ImplicitSequence<UzemnaJednotka*> containsStr(Iterator iteratorBegin, Iterator iteratorEnd, string retazec) const
	{
		ImplicitSequence<UzemnaJednotka*> s2 = this->dajDataKtoreSplnajuPredikat<Iterator>(iteratorBegin, iteratorEnd, [&](UzemnaJednotka* a) {

			if (a->toString().find(retazec) != std::string::npos)
			{
				return true;
			}
			return false;
			});

		return s2;
	}

	template<typename Iterator>
	ImplicitSequence<UzemnaJednotka*> hasType(Iterator iteratorBegin, Iterator iteratorEnd, string typ) const
	{
		ImplicitSequence<UzemnaJednotka*> s3 = this->dajDataKtoreSplnajuPredikat<Iterator>(iteratorBegin, iteratorEnd, [&](UzemnaJednotka* a) {

			if (a->getType() == typ)
			{
				return true;
			}
			return false;
			});

		return s3;
	}*/
};

