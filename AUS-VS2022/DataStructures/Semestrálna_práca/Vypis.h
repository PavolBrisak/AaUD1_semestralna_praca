#pragma once
#include "libds/amt/implicit_sequence.h"
using namespace ds::amt;
using namespace  std;


class Vypis
{
public:
	Vypis();
	~Vypis();
	void vypisMoznostiSekvencie();
	void vypisMoznostiHierarchia();
	void vypisMoznostiTabulka();
	void vypisMoznostiPredikatov();
	void vypisMoznostiPredikatovSek();
	void vypisMoznostiAplikacie();
	void vypisMoznostiTriedenia();

	template<typename Iterator>
	void vypisUdajeStruktury(Iterator iteratorBegin, Iterator iteratorEnd)
	{
		while (iteratorBegin != iteratorEnd) {
			auto& pomocna = *iteratorBegin;
			(pomocna)->vypis();
			++iteratorBegin;
		}
	}

	template<typename Struktura>
	void vypisSynov(Struktura struktura)
	{
		if (struktura->size() != 0) {
			for (int i = 0; i < struktura->size(); ++i)
			{
				cout << "  " << i << "  ";
				struktura->access(i)->data_->data_->vypis();
			}
		} else
		{
			cout << "Nach�dza� sa v liste. Nem� dostupn�ch �iadnych synov!! " << endl;
		}
	}

	template<typename Iterator>
	void vypisUdajeTabulky(Iterator iteratorBegin, Iterator iteratorEnd)
	{
		if (iteratorBegin == iteratorEnd)
		{
			cout << "Tabu�ka dan� �daj u� neobsahuje" << endl;
		}
		else {
			while (iteratorBegin != iteratorEnd) {
				auto& pomocna = *iteratorBegin;
				(pomocna)->vypis();
				++iteratorBegin;
			}
		}
	}
};

