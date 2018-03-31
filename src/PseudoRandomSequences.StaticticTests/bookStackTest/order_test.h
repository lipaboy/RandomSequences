#ifndef ORDERTEST_H
#define ORDERTEST_H

#include <vector>
#include <unordered_map>

#include <iostream>

namespace PseudoRandomSequences {

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::pair;

class OrderTest {
public:
    using WordStorageType = unsigned int;
    using size_type = size_t;       //! Be careful to use unsigned int
    using FrequencyType = size_type;
    struct NodeType { WordStorageType word; FrequencyType freq; };
    using OrderContainerType = vector<NodeType>;
    using WordMapType = vector<size_type>;  // word -> Node index
    using BorderContainerType = unordered_map<size_type, size_type>; // Set(freq) -> border position
    using BorderIterator = BorderContainerType::iterator;

    using NodeRef = NodeType &;

public:
    OrderTest() : dimension_(0), orderContainer_(1, {0, 0}),
        wordMap_(1, 0)
    {
        borders_[0] = 0;
    }

    void initialize(size_type dimension) {
        borders_.clear();

        size_type size = 1 << dimension;
        dimension_ = dimension;
        orderContainer_.resize(size);
        wordMap_.reserve(size);
        // TODO: think about not resize borders_ array. Maybe reserve?

        borders_[0] = 0;
        for (WordStorageType word = 0; word < size; word++) {
            orderContainer_[word] = { word, 0 };
            wordMap_[word] = word;
        }
    }

    // test sequence of bits
    template <class IterType>
    void test(IterType epsilonBegin, IterType epsilonEnd) {
        // TODO: use iterator_traits

        WordStorageType word = 0;
        int bitpos = 0;
        for (IterType iter = epsilonBegin; iter != epsilonEnd; iter++) {
            auto bit = *iter;
            word |= (1 << (bitpos++)) * bit;
            if (bitpos >= dimension()) {
                // find node of word
                auto index = wordMap_[word];
                NodeRef node = orderContainer_[index];
                // update frequence of word
                auto prevFreq = node.freq;
                node.freq++;
                auto nextFreq = node.freq;

                // TODO: optimize with insert
                // find border of Set(nextFreq)
                BorderIterator newFreqBorderIter = borders_.find(nextFreq);
                // create new Set(nextFreq) if such one doesn't exist
                if (newFreqBorderIter == borders_.end())
                    newFreqBorderIter = borders_.emplace(nextFreq, 0).first; // create empty Set

                // get upper boundary node of previous Set
                NodeRef upperNodeOfPreviousSet = orderContainer_[ newFreqBorderIter->second ];
                std::swap(node, upperNodeOfPreviousSet);

                // update border of Set(nextFreq)
                ++(newFreqBorderIter->second);

                // remove Set(prevFreq) if it's empty
                BorderIterator oldFreqBorderIter = borders_.find(prevFreq);
                if ((*newFreqBorderIter) == (*oldFreqBorderIter))
                    borders_.erase(oldFreqBorderIter);

                bitpos = 0;
                word = 0;
            }
        }
    }

    size_type dimension() const { return dimension_; }

private:
    size_type dimension_;
    OrderContainerType orderContainer_;
    WordMapType wordMap_;
    // border position refers to next elem after last one of set that has such border
    BorderContainerType borders_;
};

}

#endif // ORDERTEST_H
