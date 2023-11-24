#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/implicit_hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <functional>
#include <random>

namespace ds::adt {

	template <typename K, typename T>
	struct TableItem
	{
		K key_;
		T data_;

		bool operator==(const TableItem<K, T>& other) { return key_ == other.key_ && data_ == other.data_; }
		bool operator!=(const TableItem<K, T>& other) { return key_ != other.key_; }
	};

	template <typename K, typename T>
	using TabItem = TableItem<K, T>;

	//----------

	template <typename K, typename T>
	class Table :
		virtual public ADT
	{
	public:
		virtual void insert(K key, T data) = 0;
		virtual T& find(K key);
		virtual bool tryFind(K key, T*& data) = 0;
		virtual bool contains(K key);
		virtual T remove(K key) = 0;
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class SequenceTable :
		public Table<K, T>,
		public ADS<TabItem<K, T>>
	{
	public:
		SequenceTable();
		SequenceTable(const SequenceTable& other);

		bool tryFind(K key, T*& data) override;

	public:
		using BlockType = typename SequenceType::BlockType;
		using IteratorType = typename SequenceType::IteratorType;

		virtual BlockType* findBlockWithKey(K key) = 0;

		IteratorType begin();
		IteratorType end();

	protected:
		SequenceType* getSequence();
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class UnsortedSequenceTable :
		public SequenceTable<K, T, SequenceType>
	{
	protected:
		typename SequenceType::BlockType* findBlockWithKey(K key) override;
	};

	template <typename K, typename T, typename SequenceType>
	using UnsortedSTab = UnsortedSequenceTable<K, T, SequenceType>;

	//----------

	template <typename K, typename T>
	class UnsortedImplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::IS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		T remove(K key) override;

	private:
		using BlockType = typename amt::IS<TabItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedISTab = UnsortedImplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class UnsortedExplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::SinglyLS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		T remove(K key) override;

	private:
		using BlockType = typename amt::SinglyLS<TabItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedESTab = UnsortedExplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class SortedSequenceTable :
		public SequenceTable<K, T, amt::IS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		void vlozBezKontroly(K key, T data);
		T remove(K key) override;

	protected:
		using BlockType = typename amt::IS<TabItem<K, T>>::BlockType;

		BlockType* findBlockWithKey(K key) override;

	private:
		bool tryFindBlockWithKey(K key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock);
		bool skusNajstBlokSKlucomDuplicitne(K key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock);
	};

	template <typename K, typename T>
	using SortedSTab = SortedSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class HashTable :
		public Table<K, T>,
		public AUMS<TabItem<K, T>>
	{
	public:
		using HashFunctionType = std::function<size_t(K)>;

	public:
		HashTable();
		HashTable(const HashTable& other);
		HashTable(HashFunctionType hashFunction, size_t capacity);
		~HashTable();

		ADT& assign(const ADT& other) override;
		bool equals(const ADT& other) override;
		void clear() override;
		size_t size() const override;
		bool isEmpty() const override;

		void insert(K key, T data) override;
		bool tryFind(K key, T*& data) override;
		T remove(K key) override;

	private:
		using SynonymTable = UnsortedESTab<K, T>;
		using SynonymTableIterator = typename SynonymTable::IteratorType;
		using PrimaryRegionIterator = typename amt::IS<SynonymTable*>::IteratorType;

	private:
		static const size_t CAPACITY = 100;

	private:
		amt::IS<SynonymTable*>* primaryRegion_;
		HashFunctionType hashFunction_;
		size_t size_;

	public:
		class HashTableIterator
		{
		public:
			HashTableIterator(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast);
			HashTableIterator(const HashTableIterator& other);
			~HashTableIterator();
			HashTableIterator& operator++();
			HashTableIterator operator++(int);
			bool operator==(const HashTableIterator& other) const;
			bool operator!=(const HashTableIterator& other) const;
			TabItem<K, T>& operator*();

		private:
			PrimaryRegionIterator* tablesCurrent_;
			PrimaryRegionIterator* tablesLast_;
			SynonymTableIterator* synonymIterator_;
		};

		using IteratorType = HashTableIterator;

		IteratorType begin() const;

		IteratorType end() const;
	};

	//----------

	template <typename K, typename T, typename BlockType>
	class GeneralBinarySearchTree :
		public Table<K, T>,
		public ADS<TabItem<K, T>>
	{
	public:
		//using IteratorType = amt::BinaryEH<BlockType>;
		using IteratorType = typename amt::BinaryEH<BlockType>::IteratorType;

	public:
		GeneralBinarySearchTree();
		GeneralBinarySearchTree(const GeneralBinarySearchTree& other);
		~GeneralBinarySearchTree();

		size_t size() const override;
		void clear();

		void insert(K key, T data) override;
		void insertUnsafe(K key, T data);
		bool tryFind(K key, T*& data) override;
		T remove(K key) override;
		T removeUnsafe(K key);

		IteratorType begin() const;
		IteratorType end() const;

	protected:
		using BVSNodeType = typename amt::BinaryEH<BlockType>::BlockType;

		amt::BinaryEH<BlockType>* getHierarchy() const;

		virtual BVSNodeType* findNodeWithRelation(K key);
		virtual BVSNodeType& insertNode(K key, BVSNodeType* relative);
		virtual void removeNode(BVSNodeType* node);
		virtual void balanceTree(BVSNodeType* node) { }

		bool tryFindNodeWithKey(K key, BVSNodeType*& node) const;
		bool tryFindNodeWithKeyUnsafeInsert(K key, BVSNodeType*& node) const;
		bool tryFindNodeWithKeyUnsafeRemove(K key, BVSNodeType*& node) const;

		void rotateLeft(BVSNodeType* node);
		void rotateRight(BVSNodeType* node);

	private:
		size_t size_;
	};

	//----------

	template <typename K, typename T>
	class BinarySearchTree :
		public GeneralBinarySearchTree<K, T, TabItem<K, T>>
	{
	};

	//----------

	template <typename K, typename T>
	struct TreapItem :
		public TabItem<K, T>
	{
		int priority_;
	};

	template <typename K, typename T>
	class Treap :
		public GeneralBinarySearchTree<K, T, TreapItem<K, T>>
	{
	public:
		Treap();

	protected:
		using BVSNodeType = typename GeneralBinarySearchTree<K, T, TreapItem<K, T>>::BVSNodeType;

		void removeNode(BVSNodeType* node) override;
		void balanceTree(BVSNodeType* node) override;

	private:
		std::default_random_engine rng_;
	};

	//----------

	template<typename K, typename T>
	T& Table<K, T>::find(K key)
	{
		T* data = nullptr;
		if (!this->tryFind(key, data))
		{
			throw structure_error("No such key!");
		}
		return *data;
	}

	template<typename K, typename T>
	bool Table<K, T>::contains(K key)
	{
		T* data = nullptr;
		return this->tryFind(key, data);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable() :
		ADS<TabItem<K, T>>(new SequenceType())
	{
	}

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable(const SequenceTable& other) :
		ADS<TabItem<K, T>>(new SequenceType(), other)
	{
	}

	template<typename K, typename T, typename SequenceType>
	bool SequenceTable<K, T, SequenceType>::tryFind(K key, T*& data)
	{
		auto* block = this->findBlockWithKey(key);
		if (block == nullptr)
		{
			return false;
		}
		data = &block->data_.data_;
		return true;
	}

	template <typename K, typename T, typename SequenceType>
	auto SequenceTable<K, T, SequenceType>::begin() -> IteratorType
	{
		return this->getSequence()->begin();
	}

	template <typename K, typename T, typename SequenceType>
	auto SequenceTable<K, T, SequenceType>::end() -> IteratorType
	{
		return this->getSequence()->end();
	}

	template<typename K, typename T, typename SequenceType>
	SequenceType* SequenceTable<K, T, SequenceType>::getSequence()
	{
		return dynamic_cast<SequenceType*>(this->memoryStructure_);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	typename SequenceType::BlockType* UnsortedSequenceTable<K, T, SequenceType>::findBlockWithKey(K key)
	{
		return this->getSequence()->findBlockWithProperty([&key](auto* b)
			{
				return b->data_.key_ == key;
			});
	}

	//----------

	template<typename K, typename T>
	void UnsortedImplicitSequenceTable<K, T>::insert(K key, T data)
	{
		if (this->contains(key))
		{
			this->error("Tabuľka už obsahuje prvok s daným kľúčom.");
		}
		auto& prvokTabulky = this->getSequence()->insertLast().data_;
		prvokTabulky.key_ = key;
		prvokTabulky.data_ = data;
	}

	template<typename K, typename T>
	T UnsortedImplicitSequenceTable<K, T>::remove(K key)
	{
		auto* blokSKlucom = this->findBlockWithKey(key);
		if (blokSKlucom == nullptr)
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom.");
		}
		T vysledok = blokSKlucom->data_.data_;
		auto* poslednyBlok = this->getSequence()->accessLast();
		if (blokSKlucom != poslednyBlok)
		{
			std::swap(blokSKlucom->data_, poslednyBlok->data_);
		}
		this->getSequence()->removeLast();
		return vysledok;
	}

	//----------

	template<typename K, typename T>
	void UnsortedExplicitSequenceTable<K, T>::insert(K key, T data)
	{
		if (this->contains(key))
		{
			this->error("Tabuľka už obsahuje prvok s daným kľúčom.");
		}
		TabItem<K, T> prvokTabulky = this->getSequence()->insertFirst().data_;
		prvokTabulky.key_ = key;
		prvokTabulky.data_ = data;
	}

	template<typename K, typename T>
	T UnsortedExplicitSequenceTable<K, T>::remove(K key)
	{
		BlockType* blokSKlucom = this->findBlockWithKey(key);
		if (blokSKlucom == nullptr)
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom.");
		}
		T vysledok = blokSKlucom->data_.data_;
		BlockType* prvyBlok = this->getSequence()->accessFirst();
		if (blokSKlucom != prvyBlok)
		{
			std::swap(blokSKlucom->data_, prvyBlok->data_);
		}
		this->getSequence()->removeFirst();
		return vysledok;
	}

	//----------

	template<typename K, typename T>
	void SortedSequenceTable<K, T>::insert(K key, T data)
	{
		TableItem<K, T>* dataTabulky;
		if (this->isEmpty())
		{
			dataTabulky = &this->getSequence()->insertFirst().data_;
		}
		else
		{
			BlockType* blokSKlucom = nullptr;
			if (this->tryFindBlockWithKey(key, 0, this->size(), blokSKlucom))
			{
				this->error("Tabuľka už obsahuje prvok s daným kľúčom.");
			}
			dataTabulky = (key > blokSKlucom->data_.key_)
				? &this->getSequence()->insertAfter(*blokSKlucom).data_
				: &this->getSequence()->insertBefore(*blokSKlucom).data_;
		}
		dataTabulky->key_ = key;
		dataTabulky->data_ = data;
	}

	template <typename K, typename T>
	void SortedSequenceTable<K, T>::vlozBezKontroly(K key, T data)
	{
		TableItem<K, T>* dataTabulky;
		if (this->isEmpty())
		{
			dataTabulky = &this->getSequence()->insertFirst().data_;
		}
		else
		{
			BlockType* blokSKlucom = nullptr;
			this->skusNajstBlokSKlucomDuplicitne(key, 0, this->size(), blokSKlucom);
			dataTabulky = (key >= blokSKlucom->data_.key_)
				? &this->getSequence()->insertAfter(*blokSKlucom).data_
				: &this->getSequence()->insertBefore(*blokSKlucom).data_;
		}
		dataTabulky->key_ = key;
		dataTabulky->data_ = data;
	}

	template<typename K, typename T>
	T SortedSequenceTable<K, T>::remove(K key)
	{
		BlockType* blokSKlucom = nullptr;
		if (!skusNajstBlokSKlucomDuplicitne(key, 0, this->size(), blokSKlucom))
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom.");
		}
		T vysledok = blokSKlucom->data_.data_;
		if (this->getSequence()->accessFirst() == blokSKlucom)
		{
			this->getSequence()->removeFirst();
		}
		else
		{
			this->getSequence()->removeNext(*this->getSequence()->accessPrevious(*blokSKlucom));
		}
		return vysledok;
	}

	template<typename K, typename T>
	auto SortedSequenceTable<K, T>::findBlockWithKey(K key) -> BlockType*
	{
		BlockType* blockWithKey = nullptr;
		return this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey) ? blockWithKey : nullptr;
	}

	template<typename K, typename T>
	bool SortedSequenceTable<K, T>::tryFindBlockWithKey(K key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock)
	{
		if (this->isEmpty())
		{
			lastBlock = nullptr;
			return false;
		}
		size_t indexStredu = firstIndex;
		while (firstIndex < lastIndex)
		{
			indexStredu = firstIndex + (lastIndex - firstIndex) / 2;
			lastBlock = this->getSequence()->access(indexStredu);
			if (lastBlock->data_.key_ < key)
			{
				firstIndex = indexStredu + 1;
			}
			else
			{
				if (lastBlock->data_.key_ > key)
				{
					lastIndex = indexStredu;
				}
				else
				{
					break;
				}
			}
		}
		lastBlock = this->getSequence()->access(indexStredu);
		return lastBlock->data_.key_ == key;
	}

	template <typename K, typename T>
	bool SortedSequenceTable<K, T>::skusNajstBlokSKlucomDuplicitne(K key, size_t firstIndex, size_t lastIndex,
		BlockType*& lastBlock)
	{
		if (this->isEmpty())
		{
			lastBlock = nullptr;
			return false;
		}
		size_t indexStredu = firstIndex;
		while (firstIndex < lastIndex)
		{
			indexStredu = firstIndex + (lastIndex - firstIndex) / 2;
			lastBlock = this->getSequence()->access(indexStredu);
			if (lastBlock->data_.key_ <= key)
			{
				firstIndex = indexStredu + 1;
			}
			else
			{
				lastIndex = indexStredu;
			}
		}
		lastBlock = this->getSequence()->access(indexStredu);
		//return lastBlock->data_.key_ == key;
		if (lastBlock->data_.key_ == key)
		{
			return true;
		}
		else
		{
			if (indexStredu > 0 && this->getSequence()->access(indexStredu - 1)->data_.key_ == key)
			{
				lastBlock = this->getSequence()->access(indexStredu - 1);
				return true;
			}
			else if (indexStredu < this->size() - 1 && this->getSequence()->access(indexStredu + 1)->data_.key_ == key)
			{
				lastBlock = this->getSequence()->access(indexStredu + 1);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	//----------

	template<typename K, typename T>
	HashTable<K, T>::HashTable() :
		HashTable([](K key) { return std::hash<K>()(key); }, CAPACITY)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTable(const HashTable& other) :
		primaryRegion_(new amt::IS<SynonymTable*>(other.primaryRegion_->size(), true)),
		hashFunction_(other.hashFunction_),
		size_(0)
	{
		assign(other);
	}

	template<typename K, typename T>
	HashTable<K, T>::HashTable(HashFunctionType hashFunction, size_t capacity) :
		primaryRegion_(new amt::IS<SynonymTable*>(capacity, true)),
		hashFunction_(hashFunction),
		size_(0)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::~HashTable()
	{
		this->clear();
		delete primaryRegion_;
	}

	template <typename K, typename T>
	ADT& HashTable<K, T>::assign(const ADT& other)
	{
		if (this != &other)
		{
			const HashTable& otherTable = dynamic_cast<const HashTable&>(other);
			this->clear();
			for (TabItem<K, T>& otherItem : otherTable)
			{
				this->insert(otherItem.key_, otherItem.data_);
			}
		}

		return *this;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::equals(const ADT& other)
	{
		if (this == &other) { return true; }
		if (this->size() != other.size()) { return false; }

		const HashTable& otherTable = dynamic_cast<const HashTable&>(other);
		for (TabItem<K, T>& otherItem : otherTable)
		{
			T* otherData = nullptr;
			if (!this->tryFind(otherItem.key_, otherData) || *otherData != otherItem.data_)
			{
				return false;
			}
		}
		return true;
	}

	template <typename K, typename T>
	void HashTable<K, T>::clear()
	{
		size_ = 0;
		primaryRegion_->processAllBlocksForward([](typename amt::IS<SynonymTable*>::BlockType* blokSynoným)
			{
				delete blokSynoným->data_;
				blokSynoným->data_ = nullptr;
			});
	}

	template <typename K, typename T>
	size_t HashTable<K, T>::size() const
	{
		return size_;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::isEmpty() const
	{
		return size() == 0;
	}

	template <typename K, typename T>
	void HashTable<K, T>::insert(K key, T data)
	{
		size_t index = this->hashFunction_(key) % this->primaryRegion_->size();
		SynonymTable* synonyma = this->primaryRegion_->access(index)->data_;
		if (synonyma == nullptr)
		{
			synonyma = new SynonymTable();
			this->primaryRegion_->access(index)->data_ = synonyma;
		}
		synonyma->insert(key, data);
		++size_;
	}
	
	template <typename K, typename T>
	bool HashTable<K, T>::tryFind(K key, T*& data)
	{
		size_t index = this->hashFunction_(key) % this->primaryRegion_->size();
		SynonymTable* synonyma = this->primaryRegion_->access(index)->data_;
		if (synonyma == nullptr)
		{
			return false;
		}
		return synonyma->tryFind(key, data);
	}

	template <typename K, typename T>
	T HashTable<K, T>::remove(K key)
	{
		size_t index = this->hashFunction_(key) % this->primaryRegion_->size();
		SynonymTable* synonyma = this->primaryRegion_->access(index)->data_;
		if (synonyma == nullptr)
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom!");
		}
		T prvok = synonyma->remove(key);
		if (synonyma->isEmpty())
		{
			delete synonyma;
			this->primaryRegion_->access(index)->data_ = nullptr;
		}
		--size_;
		return prvok;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast) :
		tablesCurrent_(tablesFirst),
		tablesLast_(tablesLast)
	{
		while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr)
		{
			++(*tablesCurrent_);
		}
		synonymIterator_ = *tablesCurrent_ != *tablesLast_
			? new SynonymTableIterator((**tablesCurrent_)->begin())
			: nullptr;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(const HashTableIterator& other) :
		tablesCurrent_(other.tablesCurrent_),
		tablesLast_(other.tablesLast_),
		synonymIterator_(other.synonymIterator_)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::~HashTableIterator()
	{
		delete tablesCurrent_;
		delete tablesLast_;
		delete synonymIterator_;
	}

	template <typename K, typename T>
	auto HashTable<K, T>::HashTableIterator::operator++() -> HashTableIterator&
	{
		++(*synonymIterator_);
		SynonymTableIterator oldIt = *synonymIterator_;
		if (!(oldIt != (**tablesCurrent_)->end()))
		{
			do
			{
				++(*tablesCurrent_);
			} while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr);
			delete synonymIterator_;
			synonymIterator_ = nullptr;
			if (*tablesCurrent_ != *tablesLast_)
			{
				synonymIterator_ = new SynonymTableIterator((**tablesCurrent_)->begin());
			}
		}
		return *this;
	}

	template <typename K, typename T>
	auto HashTable<K, T>::HashTableIterator::operator++(int) -> HashTableIterator
	{
		HashTableIterator tmp(*this);
		operator++();
		return tmp;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator==(const HashTableIterator& other) const
	{
		return synonymIterator_ == other.synonymIterator_ ||
			(synonymIterator_ != nullptr &&
				other.synonymIterator_ != nullptr &&
				*synonymIterator_ == *(other.synonymIterator_));
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator!=(const HashTableIterator& other) const
	{
		return !(*this == other);
	}

	template <typename K, typename T>
	TabItem<K, T>& HashTable<K, T>::HashTableIterator::operator*()
	{
		return (**synonymIterator_);
	}

	//----------

	template <typename K, typename T>
	auto HashTable<K, T>::begin() const -> IteratorType
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->begin()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	template <typename K, typename T>
	auto HashTable<K, T>::end() const -> IteratorType
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->end()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::GeneralBinarySearchTree() :
		ADS<TabItem<K, T>>(new amt::BinaryEH<BlockType>()),
		size_(0)
	{
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::GeneralBinarySearchTree(const GeneralBinarySearchTree& other) :
		ADS<TabItem<K, T>>(new amt::BinaryEH<BlockType>(), other),
		size_(other.size_)
	{
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::~GeneralBinarySearchTree()
	{
		size_ = 0;
	}

	template<typename K, typename T, typename BlockType>
	size_t GeneralBinarySearchTree<K, T, BlockType>::size() const
	{
		return size_;
	}

	template <typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::clear()
	{
		ADS<TableItem<K, T>>::clear();
		//this->getHierarchy()->clear();
		size_ = 0;
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::insert(K key, T data)
	{
		BVSNodeType* novyVrchol = nullptr;
		if (this->isEmpty())
		{
			novyVrchol = &this->getHierarchy()->emplaceRoot();
		}
		else
		{
			BVSNodeType* otec = nullptr;
			if (this->tryFindNodeWithKey(key, otec))
			{
				this->error("Tabuľka už obsahuje prvok s daným kľúčom!");
			}
			novyVrchol = &this->insertNode(key, otec);
		}
		novyVrchol->data_.key_ = key;
		novyVrchol->data_.data_ = data;
		++size_;
		this->balanceTree(novyVrchol);
	}

	template <typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::insertUnsafe(K key, T data)
	{
		BVSNodeType* novyVrchol = nullptr;
		if (this->isEmpty())
		{
			novyVrchol = &this->getHierarchy()->emplaceRoot();
		}
		else
		{
			BVSNodeType* otec = nullptr;
			this->tryFindNodeWithKeyUnsafeInsert(key, otec);
			novyVrchol = &this->insertNode(key, otec);
		}
		novyVrchol->data_.key_ = key;
		novyVrchol->data_.data_ = data;
		++size_;
		this->balanceTree(novyVrchol);
	}

	template<typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFind(K key, T*& data)
	{
		BVSNodeType* vrcholSKlucom = nullptr;
		if (!this->tryFindNodeWithKey(key, vrcholSKlucom))
		{
			return false;
		}
		data = &vrcholSKlucom->data_.data_;
		return true;
	}

	template<typename K, typename T, typename BlockType>
	T GeneralBinarySearchTree<K, T, BlockType>::remove(K key)
	{
		BVSNodeType* vrcholSKlucom = nullptr;
		if (!this->tryFindNodeWithKey(key, vrcholSKlucom))
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom!");
		}
		T data = vrcholSKlucom->data_.data_;
		this->removeNode(vrcholSKlucom);
		--size_;
		return data;
	}

	template <typename K, typename T, typename BlockType>
	T GeneralBinarySearchTree<K, T, BlockType>::removeUnsafe(K key)
	{
		BVSNodeType* vrcholSKlucom = nullptr;
		if (!this->tryFindNodeWithKeyUnsafeRemove(key, vrcholSKlucom))
		{
			this->error("Tabuľka neobsahuje prvok s daným kľúčom!");
		}
		T data = vrcholSKlucom->data_.data_;
		this->removeNode(vrcholSKlucom);
		--size_;
		return data;
	}

	template <typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::begin() const -> IteratorType
	{
		return this->getHierarchy()->begin();
	}

	template <typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::end() const -> IteratorType
	{
		return this->getHierarchy()->end();
	}

	template<typename K, typename T, typename BlockType>
	amt::BinaryEH<BlockType>* GeneralBinarySearchTree<K, T, BlockType>::getHierarchy() const
	{
		return dynamic_cast<amt::BinaryEH<BlockType>*>(this->memoryStructure_);
	}

	template<typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::findNodeWithRelation(K key) -> BVSNodeType*
	{
		BVSNodeType* node = nullptr;
		this->tryFindNodeWithKey(key, node);
		return node;
	}

	template<typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::insertNode(K key, BVSNodeType* relative) -> BVSNodeType&
	{
		return key >= relative->data_.key_
			? this->getHierarchy()->insertRightSon(*relative)
			: this->getHierarchy()->insertLeftSon(*relative);
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::removeNode(BVSNodeType* node)
	{

		BVSNodeType* otec = this->getHierarchy()->accessParent(*node);
		switch (this->getHierarchy()->degree(*node))
		{
		case 0: {
			if (this->getHierarchy()->isRoot(*node))
			{
				this->getHierarchy()->clear();
			}
			else
			{
				if (this->getHierarchy()->isLeftSon(*node))
				{
					this->getHierarchy()->removeLeftSon(*otec);
				}
				else
				{
					this->getHierarchy()->removeRightSon(*otec);
				}
			}
			break;
		}
		case 1:
		{
			BVSNodeType* syn = this->getHierarchy()->hasLeftSon(*node)
				? this->getHierarchy()->accessLeftSon(*node)
				: this->getHierarchy()->accessRightSon(*node);
			if (this->getHierarchy()->isLeftSon(*syn))
			{
				this->getHierarchy()->changeLeftSon(*node, nullptr);
			}
			else
			{
				this->getHierarchy()->changeRightSon(*node, nullptr);
			}
			if (this->getHierarchy()->isRoot(*node))
			{
				this->getHierarchy()->clear();
				this->getHierarchy()->changeRoot(syn);
			}
			else if (this->getHierarchy()->isLeftSon(*node))
				{
					this->getHierarchy()->removeLeftSon(*otec);
					this->getHierarchy()->changeLeftSon(*otec, syn);
				}
				else
				{
					this->getHierarchy()->removeRightSon(*otec);
					this->getHierarchy()->changeRightSon(*otec, syn);
				}
			break;
		}
		case 2: {
			BVSNodeType* prechodcaVPoradi = this->getHierarchy()->accessLeftSon(*node);
			while (this->getHierarchy()->hasRightSon(*prechodcaVPoradi))
			{
				prechodcaVPoradi = this->getHierarchy()->accessRightSon(*prechodcaVPoradi);
			}
			std::swap(node->data_, prechodcaVPoradi->data_);
			this->removeNode(prechodcaVPoradi);
			break;
		}
		}
	}

	template<typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFindNodeWithKey(K key, BVSNodeType*& node) const
	{
		if (this->isEmpty())
		{
			return false;
		}
		node = this->getHierarchy()->accessRoot();
		while (node->data_.key_ != key && !(this->getHierarchy()->isLeaf(*node)))
		{
			if (key < node->data_.key_) {
				if (node->left_)
				{
					node = this->getHierarchy()->accessLeftSon(*node);
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (node->right_)
				{
					node = this->getHierarchy()->accessRightSon(*node);
				}
				else
				{
					return false;
				}
			}
		}
		return node->data_.key_ == key;
	}

	template <typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFindNodeWithKeyUnsafeInsert(K key, BVSNodeType*& node) const
	{
		if (this->isEmpty())
		{
			return false;
		}
		node = this->getHierarchy()->accessRoot();
		while (!(this->getHierarchy()->isLeaf(*node)))
		{
			if (key < node->data_.key_) {
				
				if (node->left_)
				{
					node = this->getHierarchy()->accessLeftSon(*node);
				}
				else
				{
					break;
				}
			}
			else
			{
				if (node->right_)
				{
					node = this->getHierarchy()->accessRightSon(*node);
				}
				else
				{
					break;
				}
			}
		}
		return node->data_.key_ == key;
	}

	template <typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFindNodeWithKeyUnsafeRemove(K key, BVSNodeType*& node) const
	{
		if (this->isEmpty())
		{
			return false;
		}
		node = this->getHierarchy()->accessRoot();
		while (!(this->getHierarchy()->isLeaf(*node)))
		{
			if (key == node->data_.key_)
			{
				break;
			}
			if (key < node->data_.key_) {

				if (node->left_)
				{
					node = this->getHierarchy()->accessLeftSon(*node);
				}
				else
				{
					break;
				}
			}
			else
			{
				if (node->right_)
				{
					node = this->getHierarchy()->accessRightSon(*node);
				}
				else
				{
					break;
				}
			}
		}
		return node->data_.key_ == key;
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::rotateLeft(BVSNodeType* node)
	{
		BVSNodeType* lavySyn = this->getHierarchy()->accessLeftSon(*node);
		BVSNodeType* otec = this->getHierarchy()->accessParent(*node);
		BVSNodeType* praotec = this->getHierarchy()->accessParent(*otec);
		this->getHierarchy()->changeRightSon(*otec, nullptr);
		this->getHierarchy()->changeLeftSon(*node, nullptr);
		if (praotec != nullptr)
		{
			if (praotec->left_ == otec)
			{
				this->getHierarchy()->changeLeftSon(*praotec, node);
			}
			else
			{
				this->getHierarchy()->changeRightSon(*praotec, node);
			}
		}
		else
		{
			this->getHierarchy()->changeRoot(node);
		}
		this->getHierarchy()->changeRightSon(*otec, lavySyn);
		this->getHierarchy()->changeLeftSon(*node, otec);
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::rotateRight(BVSNodeType* node)
	{
		BVSNodeType* pravySyn = this->getHierarchy()->accessRightSon(*node);
		BVSNodeType* otec = this->getHierarchy()->accessParent(*node);
		BVSNodeType* praotec = this->getHierarchy()->accessParent(*otec);
		this->getHierarchy()->changeLeftSon(*otec, nullptr);
		this->getHierarchy()->changeRightSon(*node, nullptr);
		if (praotec != nullptr)
		{
			if (praotec->left_ == otec)
			{
				this->getHierarchy()->changeLeftSon(*praotec, node);
			}
			else
			{
				this->getHierarchy()->changeRightSon(*praotec, node);
			}
		}
		else
		{
			this->getHierarchy()->changeRoot(node);
		}
		this->getHierarchy()->changeLeftSon(*otec, pravySyn);
		this->getHierarchy()->changeRightSon(*node, otec);
	}

	//----------

	template<typename K, typename T>
	Treap<K, T>::Treap() :
		rng_(std::rand())
	{
	}

	template<typename K, typename T>
	void Treap<K, T>::removeNode(BVSNodeType* node)
	{
		node->data_.priority_ = (std::numeric_limits<int>::max)();
		while (this->getHierarchy()->degree(*node) == 2)
		{
			BVSNodeType* lavySyn = this->getHierarchy()->accessLeftSon(*node);
			BVSNodeType* pravySyn = this->getHierarchy()->accessRightSon(*node);
			if (lavySyn->data_.priority_ < pravySyn->data_.priority_)
			{
				this->rotateRight(lavySyn);
			}
			else
			{
				this->rotateLeft(pravySyn);
			}
		}
		GeneralBinarySearchTree<K, T, TreapItem<K,T>>::removeNode(node);
	}

	template<typename K, typename T>
	void Treap<K, T>::balanceTree(BVSNodeType* node)
	{
		node->data_.priority_ = rng_(); 
		BVSNodeType* otec = this->getHierarchy()->accessParent(*node);
		while (otec != nullptr && otec->data_.priority_ > node->data_.priority_)
		{
			if (this->getHierarchy()->accessLeftSon(*otec) == node)
			{
				this->rotateRight(node);
			} else
			{
				this->rotateLeft(node);
			}
			otec = this->getHierarchy()->accessParent(*node);
		}
	}
}