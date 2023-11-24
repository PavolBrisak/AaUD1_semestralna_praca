
#include "UzemnaJednotka.h"

#include <iostream>

UzemnaJednotka::UzemnaJednotka()
{
	
}

UzemnaJednotka::~UzemnaJednotka()
{

}

UzemnaJednotka::UzemnaJednotka(int sortNumber, string code, string officialTitle, string mediumTitle, string shortTitle, string note, string typ)
{
		this->sortNumber = sortNumber;
		this->code = code;
		this->officialTitle = officialTitle;
		this->mediumTitle = mediumTitle;
		this->shortTitle = shortTitle;
		this->note = note;
		this->type = typ;
}

string UzemnaJednotka::getOfficialTitle() {
	return this->officialTitle;
}

string UzemnaJednotka::getMediumTitle() {
	return this->mediumTitle;
}

string UzemnaJednotka::getShortTitle() {
	return this->shortTitle;
}

string UzemnaJednotka::getCode()
{
	return this->code;
}

string UzemnaJednotka::getType()
{
	return this->type;
}

void UzemnaJednotka::vypis() {
	//cout << this->sortNumber << " " << this->code << " " << this->officialTitle << " " << this->mediumTitle << " " << this->shortTitle << " " << this->note << endl;
	cout << this->code << " " << this->officialTitle << endl;
}

const string UzemnaJednotka::toString()
{
	return to_string(this->sortNumber) + " " + this->code + " " + this->officialTitle + " " + this->mediumTitle + " " + this->shortTitle + " " + this->note;
}

size_t UzemnaJednotka::dajPocetSamohlasok()
{
	string officialTitle = getOfficialTitle();
	size_t vowelCount = 0;

	for (char c : officialTitle)
	{
		c = std::tolower(c);

		if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
			c == 'á' || c == 'é' || c == 'í' || c == 'ó' || c == 'ú' ||
			c == 'ä' || c == 'y' || c == 'ý' || c == 'ô')
		{
			vowelCount++;
		}
	}

	return vowelCount;
}
