#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>
#include <libds/amt/implicit_hierarchy.h>
#include <cmath>
#include <functional>

namespace ds::adt {

    template <typename P, typename T>
    struct PriorityQueueItem
    {
        P priority_;
        T data_;

        bool operator==(const PriorityQueueItem<P, T>& other) { return priority_ == other.priority_ && data_ == other.data_; }
        bool operator!=(const PriorityQueueItem<P, T>& other) { return !(*this == other); }
    };

    template <typename P, typename T>
    using PQItem = PriorityQueueItem<P, T>;

    //----------

    template <typename P, typename T>
    class PriorityQueue : virtual public ADT
    {
    public:
        virtual void push(P priority, T data) = 0;
        virtual T& peek() = 0;
        virtual T pop() = 0;
    };

    template <typename P, typename T>
    using PQ = PriorityQueue<P, T>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SequencePriorityQueue : public PriorityQueue<P, T>, public ADS<PQItem<P, T>>
    {
    public:
        SequencePriorityQueue();
        SequencePriorityQueue(const SequencePriorityQueue& other);

    protected:
        SequenceType* getSequence() const;
    };

    template <typename P, typename T, typename SequenceType>
    using SPQ = SequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class UnsortedSequencePriorityQueue : public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;

    protected:
        typename SequenceType::BlockType* findHighestPriorityBlock();
    };

    template <typename P, typename T, typename SequenceType>
    using UnsortedSPQ = UnsortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SortedSequencePriorityQueue : public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;
        T pop() override;

    protected:
        virtual size_t indexOfHighestPriorityBlock() const = 0;
    };

    template <typename P, typename T, typename SequenceType>
    using SortedSPQ = SortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T>
    class UnsortedImplicitSequencePriorityQueue : public UnsortedSequencePriorityQueue<P, T, amt::IS<PQItem<P,T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedISPQ = UnsortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class UnsortedExplicitSequencePriorityQueue : public UnsortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedESPQ = UnsortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedImplicitSequencePriorityQueue : public SortedSequencePriorityQueue<P, T, amt::IS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedISPQ = SortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedExplicitSequencePriorityQueue : public SortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedESPQ = SortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class TwoLists :
        public AUMS<PQItem<P, T>>,
        public PriorityQueue<P, T>
    {
    public:
        TwoLists(size_t expectedSize);
        TwoLists(const TwoLists<P, T>& other);
        ~TwoLists();

        ADT& assign(const ADT& other) override;
        void clear() override;
        size_t size() const override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using ShortSequenceType = amt::IS<PQItem<P, T>>;
        using LongSequenceType = amt::SinglyLS<PQItem<P, T>>;
        using ShortSequenceBlockType = typename ShortSequenceType::BlockType;
        using LongSequenceBlockType = typename LongSequenceType::BlockType;

        ShortSequenceType* shortSequence_;
        LongSequenceType* longSequence_;
    };

    //----------

    template <typename P, typename T>
    class BinaryHeap : public PriorityQueue<P, T>, public ADS<PQItem<P, T>>
    {
    public:
        BinaryHeap();
        BinaryHeap(const BinaryHeap& other);
        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using HierarchyBlockType = typename amt::BinaryIH<PQItem<P, T>>::BlockType;
        amt::BinaryIH<PQItem<P, T>>* getHierarchy();
    };

    //----------

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue() : ADS<PQItem<P, T>>(new SequenceType()) {}

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue(const SequencePriorityQueue& other) : ADS<PQItem<P, T>>(new SequenceType(), other) {}

    template<typename P, typename T, typename SequenceType>
    SequenceType* SequencePriorityQueue<P, T, SequenceType>::getSequence() const
    {
        return dynamic_cast<SequenceType*>(this->memoryStructure_);
    }

    template<typename P, typename T, typename SequenceType>
    T& UnsortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty())
        {
	        this->error("Priority queue is empty!");
        }

        return this->findHighestPriorityBlock()->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    typename SequenceType::BlockType* UnsortedSequencePriorityQueue<P, T, SequenceType>::findHighestPriorityBlock()
    {
        typename SequenceType::BlockType* bestBlock = this->getSequence()->accessFirst();

        this->getSequence()->processBlocksForward(this->getSequence()->accessNext(*bestBlock), [&](typename SequenceType::BlockType* b)
            {
                if (bestBlock->data_.priority_ > b->data_.priority_)
                {
                    bestBlock = b;
                }
            });

        return bestBlock;
    }

    template<typename P, typename T, typename SequenceType>
    T& SortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty())
        {
	        this->error("Priority queue is empty!");
        }
        return this->getSequence()->access(this->indexOfHighestPriorityBlock())->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    T SortedSequencePriorityQueue<P, T, SequenceType>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Prioritny front je prazdny");
        }
        size_t index = this->indexOfHighestPriorityBlock();
        T result = this->getSequence()->access(index)->data_.data_;
        this->getSequence()->remove(index);
        return result;
    }

    template<typename P, typename T>
    void UnsortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PriorityQueueItem<P, T>& item = this->getSequence()->insertLast().data_;
        item.priority_ = priority;
        item.data_ = data;
    }

    template<typename P, typename T>
    T UnsortedImplicitSequencePriorityQueue<P, T>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Prioritny front je prazdny");
        }
        SequenceBlockType* bestBlock = this->findHighestPriorityBlock();
        T result = bestBlock->data_.data_;
        SequenceBlockType* lastBlock = this->getSequence()->accessLast();
        if (bestBlock != lastBlock)
        {
            std::swap(bestBlock->data_, lastBlock->data_);
        }
        this->getSequence()->removeLast();
        return result;
    }

    template<typename P, typename T>
    void UnsortedExplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PriorityQueueItem<P, T>& item = this->getSequence()->insertFirst().data_;
        item.priority_ = priority;
        item.data_ = data;
    }

    template<typename P, typename T>
    T UnsortedExplicitSequencePriorityQueue<P, T>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Prioritny front je prazdny");
        }
        SequenceBlockType* bestBlock = this->findHighestPriorityBlock();
        T result = bestBlock->data_.data_;
        SequenceBlockType* firstBlock = this->getSequence()->accessFirst();
        if (bestBlock != firstBlock)
        {
            std::swap(bestBlock->data_, firstBlock->data_);
        }
        this->getSequence()->removeFirst();
        return result;
    }

    template<typename P, typename T>
    void SortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>* queueData = nullptr;

        if (this->isEmpty() || priority <= this->getSequence()->accessLast()->data_.priority_)
        {
            queueData = &this->getSequence()->insertLast().data_;
        }
        else if (priority >= this->getSequence()->accessFirst()->data_.priority_)
        {
            queueData = &this->getSequence()->insertFirst().data_;
        }
        else
        {
            queueData = &this->getSequence()->insertBefore(*this->getSequence()->findBlockWithProperty(
                [&priority](SequenceBlockType* block)->bool {
                    return block->data_.priority_ <= priority;
                })).data_;
        }

        queueData->priority_ = priority;
        queueData->data_ = data;
    }

    template<typename P, typename T>
    size_t SortedImplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        return this->size() - 1;
    }

    template<typename P, typename T>
    void SortedExplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>* queueData = nullptr;

        if (this->isEmpty() || priority <= this->getSequence()->accessFirst()->data_.priority_)
        {
            queueData = &this->getSequence()->insertFirst().data_;
        }
        else if (priority >= this->getSequence()->accessLast()->data_.priority_)
        {
            queueData = &this->getSequence()->insertLast().data_;
        }
        else
        {
            SequenceBlockType* prevBlock = this->getSequence()->findPreviousToBlockWithProperty(
                [priority](SequenceBlockType* block) -> bool
                {
                    return block->data_.priority_ >= priority;
                });
            queueData = &this->getSequence()->insertAfter(*prevBlock).data_;
        }

        queueData->priority_ = priority;
        queueData->data_ = data;
    }

    template<typename P, typename T>
    size_t SortedExplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        return 0;
    }

    template<typename P, typename T>
    TwoLists<P, T>::TwoLists(size_t expectedSize):
        shortSequence_(new ShortSequenceType(std::ceil(std::sqrt(expectedSize)), false)),
        longSequence_(new LongSequenceType())
    {
    }

    template <typename P, typename T>
    TwoLists<P, T>::TwoLists(const TwoLists<P, T>& other) :
        shortSequence_(new ShortSequenceType()),
        longSequence_(new LongSequenceType())
    {
        assign(other);
    }

    template<typename P, typename T>
    TwoLists<P, T>::~TwoLists()
    {
        delete shortSequence_;
        shortSequence_ = nullptr;
        delete longSequence_;
        longSequence_ = nullptr;
    }

    template<typename P, typename T>
    ADT& TwoLists<P, T>::assign(const ADT& other)
    {
        if (this != &other) {
            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            shortSequence_->assign(*otherTwoLists.shortSequence_);
            longSequence_->assign(*otherTwoLists.longSequence_);
        }

        return *this;
    }

    template<typename P, typename T>
    void TwoLists<P, T>::clear()
    {
        shortSequence_->clear();
        longSequence_->clear();
    }

    template<typename P, typename T>
    size_t TwoLists<P, T>::size() const
    {
        return shortSequence_->size() + longSequence_->size();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::isEmpty() const
    {
        return shortSequence_->isEmpty();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::equals(const ADT& other)
    {
        if (this != &other) {
            if (this->size() != other.size()) { return false; }

            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            return shortSequence_->equals(*otherTwoLists.shortSequence_) && longSequence_->equals(*otherTwoLists.longSequence_);
        }
        else
        {
            return true;
        }
    }

    template<typename P, typename T>
    void TwoLists<P, T>::push(P priority, T data)
    {
        PriorityQueueItem<P, T> dataFrontu = nullptr;
        if (longSequence_->size() == 0 || !(shortSequence_->isEmpty()) && shortSequence_->accessFirst()->data_.priority_ > priority)
        {
	        if (shortSequence_->size() == shortSequence_->getCapacity())
	        {
                ShortSequenceBlockType* blokKratkej = shortSequence_->accessFirst();
                LongSequenceBlockType* blokDlhej = longSequence_->insertLast();
                blokDlhej->data_.priority_ = blokKratkej->data_.priority_;
                blokDlhej->data_.data_ = blokKratkej->data_.data_;
                shortSequence_->removeLast();
	        }
            if (shortSequence_->isEmpty() || priority < shortSequence_->accessLast()->data_.priority_)
            {
                dataFrontu = &(shortSequence_->insertLast().data_);
            }
            else
            {
                if (priority > shortSequence_->accessFirst()->data_.priority_)
                {
                    dataFrontu = &(shortSequence_->insertFirst().data_);
                }
                else
                {
                    dataFrontu = &(shortSequence_->insertBefore(shortSequence_->findBlockWithProperty([&](ShortSequenceBlockType block)
                        {
                            return block.data_.priority_ <= priority;
                        })).data_);
                }
            }
        }
        else
        {
            dataFrontu = &(longSequence_->insertLast().data_);
        }
        dataFrontu.priority_ = priority;
        dataFrontu.data_ = data;
    }

    template<typename P, typename T>
    T& TwoLists<P, T>::peek()
    {
        if (shortSequence_->isEmpty())
        {
            this->error("Prioritny front je prazdny!");
        }
        else
        {
            return shortSequence_->accessLast()->data_.data_;
        }
    }

    template<typename P, typename T>
    T TwoLists<P, T>::pop()
    {
        if (isEmpty())
        {
	        this->error("Prioritny front je prazdny!");
        }
    	T vysledok = shortSequence_->accessLast()->data_.data_;
        shortSequence_->removeLast();
        if (shortSequence_->size() == 0 && longSequence_->size() > 0)
        {
            SinglyLinkedList<PriorityQueueItem<P, T>>* povodnaDlahaSekvencia = longSequence_;
            longSequence_ = new SinglyLinkedList<PriorityQueueItem<P, T>>();
            shortSequence_->changeCapacity(ceil(sqrt(povodnaDlahaSekvencia->size())));
            while (!povodnaDlahaSekvencia->isEmpty())
            {
                LongSequenceBlockType blok = povodnaDlahaSekvencia->accessFirst();
                push(blok.data_.priority_, blok.data_.data_);
                povodnaDlahaSekvencia->removeFirst();
            }
            delete povodnaDlahaSekvencia;
        }
        return vysledok;
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap() :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>())
    {
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap(const BinaryHeap& other) :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>(), other)
    {
    }

    template<typename P, typename T>
    void BinaryHeap<P, T>::push(P priority, T data)
    {
        PriorityQueueItem<P, T>& dataFrontu = getHierarchy()->insertLastLeaf().data_;
        dataFrontu.priority_ = priority;
        dataFrontu.data_ = data;
        HierarchyBlockType* aktualnyBlok = getHierarchy()->accessLastLeaf();
        HierarchyBlockType* otecBLoku = getHierarchy()->accessParent(*aktualnyBlok);
        while (otecBLoku != nullptr && aktualnyBlok->data_.priority_ < otecBLoku->data_.priority_)
        {
            std::swap(aktualnyBlok->data_, otecBLoku->data_);
            aktualnyBlok = otecBLoku;
            otecBLoku = getHierarchy()->accessParent(*aktualnyBlok);
        }
    }

    template<typename P, typename T>
    T& BinaryHeap<P, T>::peek()
    {
        if (this->isEmpty())
        {
            this->error("Prioritny front je prazdny!");
        }
        return getHierarchy()->accessRoot()->data_.data_;
    }

    template<typename P, typename T>
    T BinaryHeap<P, T>::pop()
    {
        if (this->isEmpty())
        {
            this->error("Prioritny front je prazdny!");
        }
        auto* aktualnyBlok = this->getHierarchy()->accessRoot();
        T vysledok = aktualnyBlok->data_.data_;
        std::swap(aktualnyBlok->data_, this->getHierarchy()->accessLastLeaf()->data_);
        this->getHierarchy()->removeLastLeaf();
        if (!this->isEmpty())
        {
            auto findBetterSon = [this](auto* parent)
            {
                auto* leftSon = this->getHierarchy()->accessLeftSon(*parent);
                auto* rightSon = this->getHierarchy()->accessRightSon(*parent);
                return rightSon == nullptr || leftSon->data_.priority_ < rightSon->data_.priority_
                    ? leftSon
                    : rightSon;
            };
            auto* son = findBetterSon(aktualnyBlok);
            while (son != nullptr && aktualnyBlok->data_.priority_ > son->data_.priority_)
            {
                std::swap(aktualnyBlok->data_, son->data_);
                aktualnyBlok = son;
                son = findBetterSon(aktualnyBlok);
            }
        }
        return vysledok;
    }

    template<typename P, typename T>
    amt::BinaryIH<PQItem<P, T>>* BinaryHeap<P, T>::getHierarchy()
    {
        return dynamic_cast<amt::BinaryIH<PQItem<P, T>>*>(this->memoryStructure_);
    }
}
