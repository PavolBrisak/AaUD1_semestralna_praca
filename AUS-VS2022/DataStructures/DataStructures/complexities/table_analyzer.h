#pragma once
#include <complexities/complexity_analyzer.h>
#include <cmath>
#include <random>
#include "libds/adt/table.h"

namespace ds::utils {
    
    template<typename Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table> {
    protected:
        TableAnalyzer(const std::string& name) :
            ComplexityAnalyzer<Table>(name, [&](Table& table, size_t n) {
            for (size_t i = 0; i < n; i++) {
                auto key = this->dajNovyNahodnyKluc();
                auto data = 1;

                table.insert(key, data);
            }
                }), rng_(std::random_device{}())
        {
            for (int i = 0; i <= this->getStepCount() + this->getStepSize() + 1000000; ++i)
	        {
                this->kluce.push_back(i);
	        }
            this->pocetVlozenych = 0;
            std::shuffle(kluce.begin(), kluce.end(), this->rng_);
        }

        size_t randomKey_ = 0;
        size_t randomData_ = 1;
        std::default_random_engine rng_;
        std::vector<int> kluce;
        size_t pocetVlozenych;

        size_t dajNovyNahodnyKluc()
        {
            ++this->pocetVlozenych;
            return this->kluce.at(this->pocetVlozenych);
        }
    };

    template<typename Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        TableInsertAnalyzer(const std::string& pname) :
            TableAnalyzer<Table>(pname)
        {
        }

        void beforeOperation(Table& structure) override
        {
	        try
	        {
                this->randomKey_ = this->dajNovyNahodnyKluc();
	        }
	        catch (std::runtime_error e)
	        {
                throw e;
	        }
        }

    	void executeOperation(Table& structure) override
        {
        	structure.insert(this->randomKey_, this->randomData_);
        }
    };

    template<typename Table>
    class TableSelectAnalyzer : public TableAnalyzer<Table>
    {
    public:
        TableSelectAnalyzer(const std::string& pname) :
            TableAnalyzer<Table>(pname)
        {
        }

        void beforeOperation(Table& structure) override
        {
            std::uniform_int_distribution<size_t> dist(0, this->pocetVlozenych);
            size_t index = dist(this->rng_);
            this->randomKey_ = this->kluce.at(index);
        }
        
        void executeOperation(Table& structure) override
        {
            structure.find(this->randomKey_);
        }
    };

    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer() : CompositeAnalyzer("Table")
        {
            this->addAnalyzer(std::make_unique < TableInsertAnalyzer <adt::Treap<size_t, int>>>("treap_insert"));
            this->addAnalyzer(std::make_unique < TableSelectAnalyzer <adt::Treap<size_t, int>>>("treap_select"));
        }
    };
}


