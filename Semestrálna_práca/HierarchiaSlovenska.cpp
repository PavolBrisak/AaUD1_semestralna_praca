#include <libds/amt/implicit_sequence.h>
#include "HierarchiaSlovenska.h"

HierarchiaSlovenska::HierarchiaSlovenska()
{
	this->hierarchia = new MultiWayExplicitHierarchy<UzemnaJednotka*>;
	this->slovensko = new UzemnaJednotka(0, "1", "Slovensko", "Slovensko", "Slovensko", "SK", "");
}

HierarchiaSlovenska::~HierarchiaSlovenska()
{
	this->hierarchia->clear();
	delete this->slovensko;
	delete this->hierarchia;
}

MultiWayExplicitHierarchy<UzemnaJednotka*>& HierarchiaSlovenska::nacitajHierarchiu(ImplicitSequence<UzemnaJednotka>& kraje, ImplicitSequence<UzemnaJednotka>& okresy, ImplicitSequence<UzemnaJednotka>& obce)
{
	this->hierarchia->emplaceRoot().data_ = this->slovensko;

	auto iteratorKraje = kraje.begin();
	auto iteratorKrajeEnd = kraje.end();
	auto iteratorOkresy = okresy.begin();
	auto iteratorOkresyEnd = okresy.end();
	auto iteratorObce = obce.begin();
	auto iteratorObceEnd = obce.end();

	for (int i = 0; i < kraje.size() && iteratorKraje != iteratorKrajeEnd; ++i)
	{
		auto& kraj1 = this->hierarchia->emplaceSon(*this->hierarchia->accessRoot(), i);
		kraj1.data_ = &*iteratorKraje;
		string pomocnyKraje = iteratorKraje.operator*().toString().substr(iteratorKraje.operator*().toString().length() - 5);

		//kontrola pre zahranicie
		if (pomocnyKraje == "*****")
		{
			pomocnyKraje = "SKZZZ";
		}

		int indexOkresy = 0;
		while (iteratorOkresy != iteratorOkresyEnd && iteratorOkresy.operator*().getCode().substr(0, 5) == pomocnyKraje)
		{
			auto& okres1 = this->hierarchia->emplaceSon(kraj1, indexOkresy);
			okres1.data_ = &*iteratorOkresy;
			int indexObce = 0;
			while (iteratorObce != iteratorObceEnd && iteratorObce.operator * ().getCode().substr(0, 6) == iteratorOkresy.operator*().getCode())
			{
				auto& obec1 = this->hierarchia->emplaceSon(okres1, indexObce);
				obec1.data_ = &*iteratorObce;
				++iteratorObce;
				++indexObce;
			}
			++iteratorOkresy;
			++indexOkresy;
		}
		++iteratorKraje;
	}
	return *this->hierarchia;
}
