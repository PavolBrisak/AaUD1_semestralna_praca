
#include "Vstup.h"

#include <fstream>
using namespace std;

Vstup::Vstup()
{
	this->sekvencia = new ImplicitSequence<UzemnaJednotka>();
}

Vstup::~Vstup()
{
	this->sekvencia->clear();
	delete this->sekvencia;
}

ImplicitSequence<UzemnaJednotka>& Vstup::nacitajUdaje(string nazovSuboru) {
	this->sekvencia->clear();
	string riadok;
	string slovo;
	string typ;
	int index = 0;
	file.open(nazovSuboru, ios::in);
	if (file.is_open()) {
		getline(file, riadok);
		while (!file.eof())
		{
			getline(file, riadok);
			if (!riadok.empty()) {
				stringstream s_stream(riadok);
				getline(s_stream, slovo, ';');
				int sortNumber = stoi(slovo);

				getline(s_stream, slovo, ';');
				string code = slovo;

				getline(s_stream, slovo, ';');
				string officialTitle = slovo;

				getline(s_stream, slovo, ';');
				string mediumTitle = slovo;

				getline(s_stream, slovo, ';');
				string shortTitle = slovo;

				getline(s_stream, slovo, ';');
				string note = slovo;

				if (nazovSuboru == "kraje.csv")
				{
					typ = "kraj";
				}
				if (nazovSuboru == "okresy.csv")
				{
					typ = "okres";
				}
				if (nazovSuboru == "obce.csv")
				{
					typ = "obec";
				}

				this->sekvencia->insert(index).data_ = UzemnaJednotka(sortNumber, code, officialTitle, mediumTitle, shortTitle, note, typ);
				index++;
			}
		}
	} else {
		cout << "Súbor sa neotvoril!";
		return *this->sekvencia;
	}
	file.close();
	return *this->sekvencia;
}
