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

template <class WordStorageType = unsigned int>       //word is binary
class OrderTest {
public:
    typedef size_t size_type;       //! Be careful to use unsigned int
    typedef size_type FrequencyType;
    struct NodeType { WordStorageType word; FrequencyType freq; };
    typedef vector<NodeType> OrderContainerType;
    typedef vector<size_type> WordMapType;  // word -> Node index
    typedef unordered_map<size_type, size_type> BorderContainerType; // Set(i) -> "border pointer"
    typedef BorderContainerType::iterator BorderIterator;

    typedef WordStorageType & WordRef;

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

    // TODO: use iterator_traits
    template <class IterType>
    void test(IterType epsilonBegin, IterType epsilonEnd) {
        WordStorageType word = 0;
        int bitpos = 0;
        for (IterType iter = epsilonBegin; iter != epsilonEnd; iter++) {
            auto bit = *iter;
            word |= (1 << (bitpos++)) * bit;
            if (bitpos >= dimension()) {
                auto index = wordMap_[word];
                WordRef node = orderContainer_[index];
                auto oldFreq = node.freq;
                node.freq++;
                auto newFreq = node.freq;

                // TODO: optimize with insert
                BorderIterator newFreqBorderIter = border_.find(newFreq);
                if (newFreqBorderIter == borders_.end())
                    newFreqBorderIter = borders_.insert(newFreq, 0).first;  // make the new set empty

                WordRef upperSetNode = orderContainer_[ *newFreqBorderIter ];
                std::swap(node, upperSetNode);

                // update border of set
                ++(*newFreqBorderIter);

                // remove border of empty set
                BorderIterator oldFreqBorderIter = border.find(oldFreq);
                if ((*newFreqBorderIter) == (*oldFreqBorderIter))
                    border_.extract(oldFreqBorderIter);

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
    // index refers to next elem after last one of set that has such border
    BorderContainerType borders_;
};

}

#endif // ORDERTEST_H
