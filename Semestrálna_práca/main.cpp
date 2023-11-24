#include <Windows.h>
#include <libds/heap_monitor.h>
#include <string>
#include <libds/amt/implicit_sequence.h>
#include <iostream>
#include "Algoritmus.h"
#include "HierarchiaSlovenska.h"
#include "SekvenciaUzemnychJednotiek.h"
#include "TabulkaUzemnychJednotiek.h"
#include "TriediaciAlgoritmus.h"
#include "Vstup.h"
#include "Vypis.h"
#include "UzemnaJednotka.h"
#include "libds/adt/table.h"

using namespace ds::amt;
using namespace std;

int main() {
	initHeapMonitor();
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);
	bool koniecAplikacie = false;
	string moznost;
	string nazovSuboru;
	string zacinaNaRetazec;
	string obsahujeRetazec;
	string typ;
	string typTab;
	auto algoritmus = new Algoritmus();

	auto sekKraje = new SekvenciaUzemnychJednotiek();
	auto sekOkresy = new SekvenciaUzemnychJednotiek();
	auto sekObce = new SekvenciaUzemnychJednotiek();

	auto tabulkaUJKraje = new TabulkaUzemnychJednotiek();
	auto tabulkaUJOkresy = new TabulkaUzemnychJednotiek();
	auto tabulkaUJObce = new TabulkaUzemnychJednotiek();

	auto triediaciAlgoritmus = new TriediaciALgoritmus();
	auto vstup = new Vstup();
	auto vypis = new Vypis();

	//--------------------------------
	ImplicitSequence<UzemnaJednotka> kraje = vstup->nacitajUdaje("kraje.csv");
	ImplicitSequence<UzemnaJednotka> okresy = vstup->nacitajUdaje("okresy.csv");
	ImplicitSequence<UzemnaJednotka> obce = vstup->nacitajUdaje("obce.csv");
	//--------------------------------
	ImplicitSequence<UzemnaJednotka*> sekvenciaKraje = sekKraje->nacitajSekvenciu(kraje);
	ImplicitSequence<UzemnaJednotka*> sekvenciaOkresy = sekOkresy->nacitajSekvenciu(okresy);
	ImplicitSequence<UzemnaJednotka*> sekvenciaObce = sekObce->nacitajSekvenciu(obce);
	//--------------------------------
	HierarchiaSlovenska hierarchia_slovenska;
	MultiWayExplicitHierarchy<UzemnaJednotka*> hierarchia_nacitana = hierarchia_slovenska.nacitajHierarchiu(kraje, okresy, obce);
	//--------------------------------
	Treap<string, UzemnaJednotka*> tabulkaKrajeTreap = tabulkaUJKraje->nacitajTreap(kraje);
	Treap<string, UzemnaJednotka*> tabulkaOkresyTreap = tabulkaUJOkresy->nacitajTreap(okresy);
	Treap<string, UzemnaJednotka*> tabulkaObceTreap = tabulkaUJObce->nacitajTreap(obce);

	string kluc;
	//-------------------------------

	
	while (!koniecAplikacie)
	{
		cout << "Vitajte v aplikácií, vyberte z možností:" << endl;
		vypis->vypisMoznostiAplikacie();
		cin >> moznost;
		int pomocnaInt = stoi(moznost);
		switch (pomocnaInt)
		{
			//prva uroven
		case 0:
			{
			bool koniecSekvencie = false;
				while (!koniecSekvencie)
				{
					vypis->vypisMoznostiSekvencie();
					cin >> moznost;
					int pomocnaIntSek = stoi(moznost);
					cout << "-----------------" << endl;
					switch (pomocnaIntSek)
					{
					case 0:
						{
						vypis->vypisMoznostiPredikatovSek();
						cin >> moznost;
						int vyberPredikatuSek = stoi(moznost);
							switch (vyberPredikatuSek)
							{
							case 0:
								{
								cout << "Zadajte reazec, na ktorý má údaj zaèína: ";
								cin >> zacinaNaRetazec;
								ImplicitSequence<UzemnaJednotka*> s1Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaKraje.begin(), sekvenciaKraje.end(), [&](UzemnaJednotka* a) {

									if (a->getShortTitle().substr(0, zacinaNaRetazec.size()) == zacinaNaRetazec)
									{
										return true;
									}
									return false;
									});

								if (s1Sek.size() == 0)
								{
									cout << "iadne údaje nezaèínajú na zadaný reazec '" + zacinaNaRetazec + "'" << endl;
									cout << "-----------------" << endl;
								}
								else {
									auto iteratorBegin1 = s1Sek.begin();
									auto iteratorEnd1 = s1Sek.end();
									cout << "Údaje, ktoré zaèínajú na reazec '" + zacinaNaRetazec + "'" << endl;
									cout << "-----------------" << endl;
									vypis->vypisUdajeStruktury(iteratorBegin1, iteratorEnd1);
									cout << "-----------------" << endl;
								}
								break;
								}
							case 1:
								{
								cout << "Zadajte reazec, ktorý majú údaje obsahova: ";
								cin >> obsahujeRetazec;
								ImplicitSequence<UzemnaJednotka*> s2Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaKraje.begin(), sekvenciaKraje.end(), [&](UzemnaJednotka* a) {

									if (a->toString().find(obsahujeRetazec) != std::string::npos)
									{
										return true;
									}
									return false;
									});
								if (s2Sek.size() == 0)
								{
									cout << "Žiadne údaje neobsahujú zadaný reazec '" + obsahujeRetazec + "'" << endl;
									cout << "-----------------" << endl;
								}
								else {
									auto iteratorBegin2 = s2Sek.begin();
									auto iteratorEnd2 = s2Sek.end();
									cout << "Údaje, ktoré obsahujú reazec '" + obsahujeRetazec + "'" << endl;
									cout << "-----------------" << endl;
									vypis->vypisUdajeStruktury(iteratorBegin2, iteratorEnd2);
									cout << "-----------------" << endl;
								}
								break;
								}
							default:
								cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
								cout << "-----------------" << endl;
							}
						break;
						}
					case 1:
					{
						vypis->vypisMoznostiPredikatovSek();
						cin >> moznost;
						int vyberPredikatuSek = stoi(moznost);
						switch (vyberPredikatuSek)
						{
						case 0:
						{
							cout << "Zadajte reazec, na ktorý má údaj zaèína: ";
							cin >> zacinaNaRetazec;
							ImplicitSequence<UzemnaJednotka*> s4Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaOkresy.begin(), sekvenciaOkresy.end(), [&](UzemnaJednotka* a) {

								if (a->getShortTitle().substr(0, zacinaNaRetazec.size()) == zacinaNaRetazec)
								{
									return true;
								}
								return false;
								});
							if (s4Sek.size() == 0)
							{
								cout << "iadne údaje nezaèínajú na zadaný reazec '" + zacinaNaRetazec + "'" << endl;
								cout << "-----------------" << endl;
							}
							else {
								auto iteratorBegin4 = s4Sek.begin();
								auto iteratorEnd4 = s4Sek.end();
								cout << "Údaje, ktoré zaèínajú na reazec '" + zacinaNaRetazec + "'" << endl;
								cout << "-----------------" << endl;
								vypis->vypisUdajeStruktury(iteratorBegin4, iteratorEnd4);
								cout << "-----------------" << endl;
							}
							break;
						}
						case 1:
						{
							cout << "Zadajte reazec, ktorý majú údaje obsahova: ";
							cin >> obsahujeRetazec;
							ImplicitSequence<UzemnaJednotka*> s5Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaOkresy.begin(), sekvenciaOkresy.end(), [&](UzemnaJednotka* a) {

								if (a->toString().find(obsahujeRetazec) != std::string::npos)
								{
									return true;
								}
								return false;
								});
							if (s5Sek.size() == 0)
							{
								cout << "Žiadne údaje neobsahujú zadaný reazec '" + obsahujeRetazec + "'" << endl;
								cout << "-----------------" << endl;
							}
							else {
								auto iteratorBegin5 = s5Sek.begin();
								auto iteratorEnd5 = s5Sek.end();
								cout << "Údaje, ktoré obsahujú reazec '" + obsahujeRetazec + "'" << endl;
								cout << "-----------------" << endl;
								vypis->vypisUdajeStruktury(iteratorBegin5, iteratorEnd5);
								cout << "-----------------" << endl;
							}
							break;
							}
						default:
							cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
							cout << "-----------------" << endl;
						}
						break;
						}
					case 2:
						{
						vypis->vypisMoznostiPredikatovSek();
						cin >> moznost;
						int vyberPredikatuSek = stoi(moznost);
						switch (vyberPredikatuSek)
						{
						case 0:
						{
							cout << "Zadajte reazec, na ktorý má údaj zaèína: ";
							cin >> zacinaNaRetazec;
							ImplicitSequence<UzemnaJednotka*> s7Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaObce.begin(), sekvenciaObce.end(), [&](UzemnaJednotka* a) {

								if (a->getShortTitle().substr(0, zacinaNaRetazec.size()) == zacinaNaRetazec)
								{
									return true;
								}
								return false;
								});
							if (s7Sek.size() == 0)
							{
								cout << "iadne údaje nezaèínajú na zadaný reazec '" + zacinaNaRetazec + "'" << endl;
								cout << "-----------------" << endl;
							}
							else {
								auto iteratorBegin7 = s7Sek.begin();
								auto iteratorEnd7 = s7Sek.end();
								cout << "Údaje, ktoré zaèínajú na reazec '" + zacinaNaRetazec + "'" << endl;
								cout << "-----------------" << endl;
								vypis->vypisUdajeStruktury(iteratorBegin7, iteratorEnd7);
								cout << "-----------------" << endl;
							}
							break;
						}
						case 1:
						{
							cout << "Zadajte reazec, ktorý majú údaje obsahova: ";
							cin >> obsahujeRetazec;
							ImplicitSequence<UzemnaJednotka*> s8Sek = algoritmus->dajDataKtoreSplnajuPredikat(sekvenciaObce.begin(), sekvenciaObce.end(), [&](UzemnaJednotka* a) {

								if (a->toString().find(obsahujeRetazec) != std::string::npos)
								{
									return true;
								}
								return false;
								});
							if (s8Sek.size() == 0)
							{
								cout << "Žiadne údaje neobsahujú zadaný reazec '" + obsahujeRetazec + "'" << endl;
								cout << "-----------------" << endl;
							}
							else {
								auto iteratorBegin8 = s8Sek.begin();
								auto iteratorEnd8 = s8Sek.end();
								cout << "Údaje, ktoré obsahujú reazec '" + obsahujeRetazec + "'" << endl;
								cout << "-----------------" << endl;
								vypis->vypisUdajeStruktury(iteratorBegin8, iteratorEnd8);
								cout << "-----------------" << endl;
							}
							break;
						}
						default:
							cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
							cout << "-----------------" << endl;
						}
						break;
						}
					case 3:
						{
						koniecSekvencie = true;
						break;
						}
					default:
						cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
						cout << "-----------------" << endl;
					}
				}
			break;
			}
			//druha uroven
		case 1:
			{
			bool koniecHierarchia = false;
			auto iteratorMulti = hierarchia_nacitana.beginMulti();
			while (!koniecHierarchia)
			{
				cout << "Nachádzaš sa na:" << endl;
				iteratorMulti.dajData()->vypis();
				cout << "-----------------" << endl;
				int pocetSynov = iteratorMulti.dajAktualnyVrchol()->sons_->size();
				cout << "Dostupní synovia: " << endl;
				vypis->vypisSynov(iteratorMulti.dajAktualnyVrchol()->sons_);
				cout << "-----------------" << endl;
				vypis->vypisMoznostiHierarchia();
				cin >> moznost;
				int pomocnaInt = stoi(moznost);
				cout << "-----------------" << endl;
				switch (pomocnaInt)
				{
					//chod na syna
				case 0:
				{
					if (iteratorMulti.dajAktualnyVrchol()->sons_->size() != 0) {
						cout << "Zadajte index syna (dostupné sú indexy od 0 po " << pocetSynov - 1 << ")" << endl;
						cin >> moznost;
						int pomocnaInt1 = stoi(moznost);
						cout << "-----------------" << endl;
						if (pomocnaInt1 < 0 || pomocnaInt1 >= pocetSynov)
						{
							cout << "Zadali ste neplatný index syna!!! Dostupné indexy sú od 0 po " << pocetSynov - 1 << endl;
							cout << "-----------------" << endl;
						}
						else {
							iteratorMulti.chodNaSyna(pomocnaInt1);
						}
					}
					else
					{
						cout << "Nachádzaš sa v liste, nemáš k dispozícii žiadnych synov!!" << endl;
						cout << "-----------------" << endl;
					}
					break;
				}
					//chod na otca
				case 1:
				{
					if (iteratorMulti.dajAktualnyVrchol()->parent_ != nullptr) {
						iteratorMulti.chodNaOtca();
					}
					else
					{
						cout << "Si v koreni. Nemôžeš ís na otca!!" << endl;
						cout << "-----------------" << endl;
					}
					break;
				}
				case 2:
				{
					vypis->vypisMoznostiPredikatov();
					cin >> moznost;
					int pomocnaInt2 = stoi(moznost);
					cout << "-----------------" << endl;
					switch (pomocnaInt2)
					{
					case 0:
					{
						cout << "Zadajte reazec, na ktorý má údaj zaèína: ";
						cin >> zacinaNaRetazec;
						ImplicitSequence<UzemnaJednotka*> s1 = algoritmus->dajDataKtoreSplnajuPredikat(iteratorMulti.beginPreOrder(), iteratorMulti.endPreOrder(), [&](UzemnaJednotka* a) {

							if (a->getShortTitle().substr(0, zacinaNaRetazec.size()) == zacinaNaRetazec)
							{
								return true;
							}
							return false;
							});
						if (s1.size() == 0)
						{
							cout << "iadne údaje nezaèínajú na zadaný reazec '" + zacinaNaRetazec + "'" << endl;
							cout << "-----------------" << endl;
						}
						else {
							auto iteratorBegin1 = s1.begin();
							auto iteratorEnd1 = s1.end();
							cout << "Potomkovia, ktorí zaèínajú na reazec '" + zacinaNaRetazec + "'" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorBegin1, iteratorEnd1);
							cout << "-----------------" << endl;
						}
						vypis->vypisMoznostiTriedenia();
						cin >> moznost;
						int pomocnaInt3 = stoi(moznost);
						cout << "-----------------" << endl;
						switch (pomocnaInt3) {
						case 0:
							{
							triediaciAlgoritmus->porovnajAbecedne(s1);
							auto iteratorTR1 = s1.begin();
							auto iteratorTR2 = s1.end();
							cout << "Utriedené pod¾a abecedy:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
							}
						case 1:
							{
							triediaciAlgoritmus->porovnajPocetSamohlasok(s1);
							auto iteratorTR1 = s1.begin();
							auto iteratorTR2 = s1.end();
							cout << "Utriedené pod¾a poètu samohlások:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
							}
						case 2:
							{
							cout << "Netriedili sme." << endl;
							cout << "-----------------" << endl;
							break;
							}
						default:
							cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
							cout << "-----------------" << endl;
						}
						break;
					}
					case 1:
					{
						cout << "Zadajte reazec, ktorý majú údaje obsahova: ";
						cin >> obsahujeRetazec;
						ImplicitSequence<UzemnaJednotka*> s2 = algoritmus->dajDataKtoreSplnajuPredikat(iteratorMulti.beginPreOrder(), iteratorMulti.endPreOrder(), [&](UzemnaJednotka* a) {

							if (a->toString().find(obsahujeRetazec) != std::string::npos)
							{
								return true;
							}
							return false;
							});
						if (s2.size() == 0)
						{
							cout << "Žiadne údaje neobsahujú zadaný reazec '" + obsahujeRetazec + "'" << endl;
							cout << "-----------------" << endl;
						}
						else {
							auto iteratorBegin2 = s2.begin();
							auto iteratorEnd2 = s2.end();
							cout << "Potomkovia, ktorí obsahujú reazec '" + obsahujeRetazec + "'" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorBegin2, iteratorEnd2);
							cout << "-----------------" << endl;
						}
						vypis->vypisMoznostiTriedenia();
						cin >> moznost;
						int pomocnaInt4 = stoi(moznost);
						cout << "-----------------" << endl;
						switch (pomocnaInt4) {
						case 0:
						{
							triediaciAlgoritmus->porovnajAbecedne(s2);
							auto iteratorTR1 = s2.begin();
							auto iteratorTR2 = s2.end();
							cout << "Utriedené pod¾a abecedy:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
						}
						case 1:
						{
							triediaciAlgoritmus->porovnajPocetSamohlasok(s2);
							auto iteratorTR1 = s2.begin();
							auto iteratorTR2 = s2.end();
							cout << "Utriedené pod¾a poètu samohlások:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
						}
						case 2:
							{
								break;
							}
						default:
							cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
							cout << "-----------------" << endl;
						}
						break;
					}
					case 2:
					{
						cout << "Zadajte typ, ktorým údaje majú by: ";
						cin >> typ;
						ImplicitSequence<UzemnaJednotka*> s3 = algoritmus->dajDataKtoreSplnajuPredikat(iteratorMulti.beginPreOrder(), iteratorMulti.endPreOrder(), [&](UzemnaJednotka* a) {

							if (a->getType() == typ)
							{
								return true;
							}
							return false;
							});
						if (s3.size() == 0)
						{
							cout << "Žiadne údaje nie sú typu '" + typ + "'" << endl;
							cout << "-----------------" << endl;
						}
						else {
							auto iteratorBegin3 = s3.begin();
							auto iteratorEnd3 = s3.end();
							cout << "Potomkovia, ktorí sú typu '" + typ + "'" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorBegin3, iteratorEnd3);
							cout << "-----------------" << endl;
						}
						vypis->vypisMoznostiTriedenia();
						cin >> moznost;
						int pomocnaInt5 = stoi(moznost);
						cout << "-----------------" << endl;
						switch (pomocnaInt5) {
						case 0:
						{
							triediaciAlgoritmus->porovnajAbecedne(s3);
							auto iteratorTR1 = s3.begin();
							auto iteratorTR2 = s3.end();
							cout << "Utriedené pod¾a abecedy:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
						}
						case 1:
						{
							triediaciAlgoritmus->porovnajPocetSamohlasok(s3);
							auto iteratorTR1 = s3.begin();
							auto iteratorTR2 = s3.end();
							cout << "Utriedené pod¾a poètu samohlások:" << endl;
							cout << "-----------------" << endl;
							vypis->vypisUdajeStruktury(iteratorTR1, iteratorTR2);
							cout << "-----------------" << endl;
							break;
						}
						case 2:
						{
							break;
						}
						default:
							cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
							cout << "-----------------" << endl;
						}
						break;
					}
					default:
						cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
						cout << "-----------------" << endl;
					}
					break;
				}
				case 3:
				{
					koniecHierarchia = true;
					break;
				}
				default:
					cout << "Zadali ste nesprávne èíslo príkazu!! Skúste znovu." << endl;
					cout << "-----------------" << endl;
				}
			}
			break;
			}
			//tabulky
		case 2:
			{
			cout << "Práca s tabu¾kami: " << endl;
			bool koniecTab = false;
			while (!koniecTab)
			{
				vypis->vypisMoznostiTabulka();
				cin >> moznost;
				int pomocnaInt = stoi(moznost);
				cout << "-----------------" << endl;
				switch (pomocnaInt)
				{
				case 0:
				{
					cin >> typTab;
					cout << "Zadajte názov územnej jednotky: ";
					cin.ignore();
					getline(cin, kluc);
					if (typTab == "kraj")
					{
						ImplicitSequence<UzemnaJednotka*> pomocna1 = tabulkaUJKraje->najdiPodlaKlucaVTreap(tabulkaKrajeTreap, kluc);
						auto iteratorBegPomocna = pomocna1.begin();
						auto iteratorEndPomocna = pomocna1.end();
						vypis->vypisUdajeTabulky(iteratorBegPomocna, iteratorEndPomocna);
						cout << "-------------------" << endl;
					}
					if (typTab == "okres")
					{
						ImplicitSequence<UzemnaJednotka*> pomocna2 = tabulkaUJOkresy->najdiPodlaKlucaVTreap(tabulkaOkresyTreap, kluc);
						auto iteratorBegPomocna = pomocna2.begin();
						auto iteratorEndPomocna = pomocna2.end();
						vypis->vypisUdajeTabulky(iteratorBegPomocna, iteratorEndPomocna);
						cout << "-------------------" << endl;
					}
					if (typTab == "obec")
					{
						ImplicitSequence<UzemnaJednotka*> pomocna3 = tabulkaUJObce->najdiPodlaKlucaVTreap(tabulkaObceTreap, kluc);
						auto iteratorBegPomocna = pomocna3.begin();
						auto iteratorEndPomocna = pomocna3.end();
						vypis->vypisUdajeTabulky(iteratorBegPomocna, iteratorEndPomocna);
						cout << "-------------------" << endl;
					}
					break;
				}
				case 1:
				{
					koniecTab = true;
					break;
				}
				default:
					cout << "Chybny vstup!!!" << endl;
				}
			}
			break;
			}
		case 3:
			{
			cout << "Dovidenia!" << endl;
			koniecAplikacie = true;
			break;
			}
		default:
			{
			cout << "Nesprávne vybraná možnos!! Skúste znovu." << endl;
			cout << "-------------------" << endl;
			}
		}
	}
	
	delete algoritmus;
	delete vstup;
	delete vypis;
	delete tabulkaUJKraje;
	delete tabulkaUJOkresy;
	delete tabulkaUJObce;
	delete triediaciAlgoritmus;
	delete sekKraje;
	delete sekOkresy;
	delete sekObce;

	return 0;
}