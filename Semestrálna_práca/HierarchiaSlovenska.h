#pragma once
#include "UzemnaJednotka.h"
#include "Vstup.h"
#include "libds/amt/explicit_hierarchy.h"

using namespace ds::amt;

class HierarchiaSlovenska
{
private:
	MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchia;
	UzemnaJednotka* slovensko;

public:
	HierarchiaSlovenska();
	~HierarchiaSlovenska();

	MultiWayExplicitHierarchy<UzemnaJednotka*>& nacitajHierarchiu(ImplicitSequence<UzemnaJednotka>& kraje, ImplicitSequence<UzemnaJednotka>& okresy, ImplicitSequence<UzemnaJednotka>& obce);
};

