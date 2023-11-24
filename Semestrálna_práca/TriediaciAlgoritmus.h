#pragma once

#include "UzemnaJednotka.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"
using namespace ds::amt;
using namespace ds::adt;

class TriediaciALgoritmus
{
public:
	TriediaciALgoritmus()
	{
		
	}

	template <typename DataType>
	void utriedData(ImplicitSequence<DataType>& sekvencia, std::function<bool(DataType, DataType)> komparator)
	{
		QuickSort<DataType> quickSort;
		quickSort.sort(sekvencia, komparator);
	}

	template <typename DataType>
	void porovnajAbecedne(ImplicitSequence<DataType>& sekvencia)
	{
		const std::locale local("Slovak_Slovakia.1250");
		this->utriedData<DataType>(sekvencia, [local](DataType a, DataType b) -> bool
			{
				const auto retazecA = a->getOfficialTitle();
				const auto retazecB = b->getOfficialTitle();
				const auto& coll = std::use_facet < std::collate<char>>(local);
				const int porovnanie = coll.compare(retazecA.data(), retazecA.data() + retazecA.length(), retazecB.data(), retazecB.data() + retazecB.length());
				return porovnanie < 0;
			});
	}

	template <typename DataType>
	void porovnajPocetSamohlasok(ImplicitSequence<DataType>& sekvencia)
	{
		this->utriedData<DataType>(sekvencia, [&](DataType a, DataType b) -> bool
			{
				return a->dajPocetSamohlasok() < b->dajPocetSamohlasok();
			});
	}
};
