#include "Vypis.h"
using namespace std;

Vypis::Vypis()
{
}

Vypis::~Vypis()
{
}

void Vypis::vypisMoznostiSekvencie()
{
	cout << "Zadajte ��slo konkr�tneho pr�kazu, ktor� chcete vykona�:" << endl;
	cout << "  0  Prech�dzanie krajmi" << endl;
	cout << "  1  Prech�dzanie okresmi" << endl;
	cout << "  2  Prech�dzanie obcami" << endl;
	cout << "  3  Pre ukon�enie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiHierarchia()
{
	cout << "Zadajte ��slo konkr�tneho pr�kazu, ktor� chcete vykona�:" << endl;
	cout << "  0  Presun na zadan�ho syna" << endl;
	cout << "  1  Presun na otca" << endl;
	cout << "  2  Zavolanie predik�tu nad dan�m vrcholom a jeho potomkami" << endl;
	cout << "  3  Pre ukon�enie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiTabulka()
{
	cout << "Zadajte ��slo konkr�tneho pr�kazu, ktor� chcete vykona�:" << endl;
	cout << "  0  Zadajte typ �zemnej jednotky: " << endl;
	cout << "  1  Pre ukon�enie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiPredikatov()
{
	cout << "Zadajte ��slo konkr�tneho pr�kazu, ktor� chcete vykona�:" << endl;
	cout << "  0  startsWithStr" << endl;
	cout << "  1  containsStr" << endl;
	cout << "  2  hasType" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiPredikatovSek()
{
	cout << "Zadajte ��slo konkr�tneho pr�kazu, ktor� chcete vykona�:" << endl;
	cout << "  0  startsWithStr" << endl;
	cout << "  1  containsStr" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiAplikacie()
{
	cout << "  0  pr�ca s �zemn�mi jednotkami" << endl;
	cout << "  1  prech�dzanie hierarchiou" << endl;
	cout << "  2  vyberanie z tabu�ky" << endl;
	cout << "  3  ukon�enie aplik�cie" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiTriedenia()
{
	cout << "  0  triedenie pod�a abecedy" << endl;
	cout << "  1  triedenie pod�a po�tu samohl�sok" << endl;
	cout << "  2  netrie�" << endl;
	cout << "---------------------" << endl;
}

