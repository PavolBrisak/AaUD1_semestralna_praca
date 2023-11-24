#pragma once
#include <locale>
#include <string>

using namespace std;
class UzemnaJednotka
{
private:
	int sortNumber = 0;
	string code;
	string officialTitle;
	string mediumTitle;
	string shortTitle;
	string note;
	string type;

public:
	UzemnaJednotka(int sortNumber, string code, string officialTitle, string mediumTitle, string shortTitle, string note, string typ);
	UzemnaJednotka();
	~UzemnaJednotka();
	string getOfficialTitle();
	string getMediumTitle();
	string getShortTitle();
	string getCode();
	string getType();
	void vypis();
	const string toString();
	size_t dajPocetSamohlasok();
};
