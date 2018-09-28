#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <queue>

#include <boost/math/distributions/chi_squared.hpp>

#include "statistic_chi_squared.h"
#include "i_statistical_test.h"

#include <iostream>

namespace statistical_tests_space {

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::map;         //sorted container
using std::pair;

class OrderTest : public IStatisticalTest {
public:
    using WordStorageType = unsigned int;
    using size_type = size_t;       //! Be careful to use unsigned types
    using FrequencyType = size_type;
    struct NodeType { WordStorageType word; FrequencyType freq; };
    using OrderContainerType = vector<NodeType>;
    using WordMapType = vector<size_type>;  // word -> Node index
    using BorderContainerType = map<size_type, size_type>; // Set(freq) -> border position
    using BorderIterator = BorderContainerType::iterator;
    using UpperPartContainerType = vector<FrequencyType>;

    using PValueType = double;

    using NodeRef = NodeType &;

public:
    OrderTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams), dimension_(0), orderContainer_(1, {0, 0}),
          wordMap_(1, 0), upperPartBorder_(1), encounterFreqInUpperPart_(1, 0)
    {
        borders_[0] = 0;
    }

    void initialize(size_type dimension, FrequencyType upperPartBorder)
    {
        borders_.clear();   // need to be clear because algorithm uses the lack/existence of elements

        // update fields
        dimension_ = dimension;
        size_type size = containerSize();
        upperPartBorder_ = upperPartBorder;

        // init containers
        orderContainer_.resize(size);
        wordMap_.resize(size);
        encounterFreqInUpperPart_.assign(size, 0);
        // needn't to resize the borders_ because it's map
        borders_[0] = size;     // must point to last element in orderContainer_
        for (WordStorageType word = 0; word < size; word++) {
            orderContainer_[word] = { word, 0 };
            wordMap_[word] = word;
        }
    }

    // test sequence of bits
    virtual ReturnValueType test(BoolIterator sequenceIterBegin, size_type epsilonSize)
    {
        ReturnValueType retVal;
        for (TestParameters::BookStackPair elem : getTestParameters().orderTest) {
            initialize(elem.dimension, elem.upperPart);
            retVal.push_back(parseTestResult(testSingle(sequenceIterBegin, epsilonSize)));
        }
        return retVal;
    }

private:
    PValueType testSingle(BoolIterator sequenceIterBegin, size_type epsilonSize) {
        WordStorageType word = 0;
        int bitpos = 0;
        size_type i = 0u;
        size_type upperPartCounter = 0u;

//        size_type directionChangingCount = 0u;
//        int direction = 1;

        std::queue<int> kek;    // for debugging
        for (int i = 0; i < 10; i++)
            kek.push(0);
        for (auto iter = sequenceIterBegin; i < epsilonSize; iter++, i++)
        {
            bool bit = *iter;
            kek.push(bit);
            kek.pop();
            word |= (1 << (bitpos++)) * bit;
            if (bitpos >= static_cast<int>(dimension()))
            {
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
                if (newFreqBorderIter == borders_.end()) {
                    auto iter = borders_.find(prevFreq);
                    iter++;
                    size_type borderPos;
                    if (iter == borders_.end())
                        borderPos = 0;
                    else
                        borderPos = iter->second;
                    // create empty Set that points to upper part of Set(prevFreq)
                    newFreqBorderIter = borders_.emplace(nextFreq, borderPos).first;
                }

                // get upper boundary node of Set(prevFreq)
                NodeRef upperNodeOfPreviousSet = orderContainer_[ newFreqBorderIter->second ];
                std::swap(wordMap_[node.word], wordMap_[upperNodeOfPreviousSet.word]);
                std::swap(node, upperNodeOfPreviousSet);

                // update border position of Set(nextFreq)
                ++(newFreqBorderIter->second);

                // remove Set(prevFreq) if it's empty
                BorderIterator oldFreqBorderIter = borders_.find(prevFreq);
                if (oldFreqBorderIter != borders_.end()
                        && newFreqBorderIter->second == oldFreqBorderIter->second)
                {
                    borders_.erase(oldFreqBorderIter);
                }

                // update information about element encountering in upper part
                for (FrequencyType j = 0; j < upperPartBorder_; j++) {
                    if (word == orderContainer_[j].word) {
                        ++upperPartCounter;
                        break;
                    }
                }
//                auto getDirection = [] (size_type a, size_type b) -> int {
//                    return (a == b) ? 0 : (int(a) - int(b)) / std::abs((int)a - int(b));
//                };
//                int newDirection = getDirection(encounterFreqInUpperPart_[0],
//                        encounterFreqInUpperPart_[1]);
//                if (newDirection && newDirection != direction) {
//                    direction = newDirection;
//                    ++directionChangingCount;
//                }

                bitpos = 0;
                word = 0;
            }
        }
//cout << "Direction changing count = " << directionChangingCount << endl;

        // Calculate P-value
//        auto statisticX2 = statisticChiSquared(encounterFreqInUpperPart_.begin(), encounterFreqInUpperPart_.end(),
//                                               double(upperPartBorder_) / containerSize() * (double(epsilonSize) / dimension()));

		//
//		PValueType pValue = 0;
        PValueType pValue = 1 -
            boost::math::cdf(boost::math::chi_squared_distribution<double>(1),
                //statisticX2);
                statisticChiSquared(upperPartCounter,
                                    double(upperPartBorder_) / containerSize()
                                        * (double(epsilonSize) / dimension())));
        return pValue;
    }

public:

    size_type dimension() const { return dimension_; }
    size_type containerSize() const { return static_cast<size_type>(1) << dimension(); }

private:
    size_type dimension_;
    OrderContainerType orderContainer_; // saves property of order
    WordMapType wordMap_;
    // border position refers to next elem after last one of set that has such border
    BorderContainerType borders_;   // ADVICE: you can replace ordered_map on array with size of alphabet because borders count can't be more than words

    FrequencyType upperPartBorder_;
    UpperPartContainerType encounterFreqInUpperPart_;
};

}

