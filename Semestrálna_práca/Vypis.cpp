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
	cout << "Zadajte èíslo konkrétneho príkazu, ktorý chcete vykona:" << endl;
	cout << "  0  Prechádzanie krajmi" << endl;
	cout << "  1  Prechádzanie okresmi" << endl;
	cout << "  2  Prechádzanie obcami" << endl;
	cout << "  3  Pre ukonèenie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiHierarchia()
{
	cout << "Zadajte èíslo konkrétneho príkazu, ktorý chcete vykona:" << endl;
	cout << "  0  Presun na zadaného syna" << endl;
	cout << "  1  Presun na otca" << endl;
	cout << "  2  Zavolanie predikátu nad daným vrcholom a jeho potomkami" << endl;
	cout << "  3  Pre ukonèenie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiTabulka()
{
	cout << "Zadajte èíslo konkrétneho príkazu, ktorý chcete vykona:" << endl;
	cout << "  0  Zadajte typ územnej jednotky: " << endl;
	cout << "  1  Pre ukonèenie programu" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiPredikatov()
{
	cout << "Zadajte èíslo konkrétneho príkazu, ktorý chcete vykona:" << endl;
	cout << "  0  startsWithStr" << endl;
	cout << "  1  containsStr" << endl;
	cout << "  2  hasType" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiPredikatovSek()
{
	cout << "Zadajte èíslo konkrétneho príkazu, ktorý chcete vykona:" << endl;
	cout << "  0  startsWithStr" << endl;
	cout << "  1  containsStr" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiAplikacie()
{
	cout << "  0  práca s územnými jednotkami" << endl;
	cout << "  1  prechádzanie hierarchiou" << endl;
	cout << "  2  vyberanie z tabu¾ky" << endl;
	cout << "  3  ukonèenie aplikácie" << endl;
	cout << "-----------------" << endl;
}

void Vypis::vypisMoznostiTriedenia()
{
	cout << "  0  triedenie pod¾a abecedy" << endl;
	cout << "  1  triedenie pod¾a poètu samohlások" << endl;
	cout << "  2  netrieï" << endl;
	cout << "---------------------" << endl;
}

