#pragma once
#include <complexities/complexity_analyzer.h>
#include <cmath>
#include <random>

namespace  ds::utils
{
	template<typename Structure>
	class AccessAnlayzer : public ComplexityAnalyzer<Structure>
	{

	public:
		AccessAnlayzer(
			const std::string& name,
			std::function<void(Structure&, size_t)> insertN) :
			ComplexityAnalyzer(name, insertN)
		{
		}

		void executeOperation(Structure& structure) override
		{
			structure.access(randomIndex_);
		}

		void beforeOperation(Structure& structure) override
		{
			std::uniform_int_distribution<size_t> dist(0, structure.size() - 1);

			//toto nikdy nepouzivat
			//randomIndex_ = std::rand();
			randomIndex_ = dist(rng_);

		}
	private:
		size_t randomIndex_;
		std::default_random_engine rng_;
	};

	template<typename List>
	class ListAccessAnalyzer : public AccessAnlayzer<List>
	{
	public:
		ListAccessAnalyzer(const std::string& name) :
		AccessAnlayzer(name, [](List& list, size_t n)
		{
			for (size_t i = 0; i < n; ++i)
			{
				list.insertLast(69);
			}
		})
		{
			
		}
	};
}
