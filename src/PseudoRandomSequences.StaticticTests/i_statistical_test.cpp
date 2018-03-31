#include "i_statistical_test.h"

namespace statistical_tests_space {

TestParameters::TestParameters(uint64_t EPSILON_SIZE)
    : blockFrequencyTest({ 2, //EPSILON_SIZE / 4, EPSILON_SIZE / 2
                         //16, 32, 128
                         10
                         }),
      nonOverlappingTemplateMatchingsTest({ 3, 5, 7, //14
                                          }),                    //slow test!!!!
      overlappingTemplateMatchingsTest({ 2, 6, 12 })
{
    for (int upperPart = 0; upperPart < 3; upperPart++) {
        for (uint64_t dim = 8; dim <= 32; dim *= 2) {	//8, 16, 32
            uint64_t upperPartSize = (upperPart == 0) ? 16LL
                : ((upperPart == 1) ? (1LL << (dim / 2)) : (1LL << (dim - 1)));
            if (upperPartSize > (1LL << 28))
                continue;
            bookStackTest.push_back({ upperPartSize, dim });
        }
    }

//    auto sqrtSize = uint64_t(std::floor(std::pow(EPSILON_SIZE, 0.5)));
//    auto sqrtSqrtSize = uint64_t(std::floor(std::pow(EPSILON_SIZE, 0.25)));
    linearComplexityTest = { 8, 10, //sqrtSqrtSize
                             24, //1000 //is checked on linux
                           //  sqrtSize
                           };                   //slow test!!!!
//    uint64_t logSize = uint64_t(std::floor(std::log2(EPSILON_SIZE)) - 2);
    //Choose m and n such that m < (log2 n) - 2.
    serialTest = { 2, 3, //logSize / 2, logSize
                 5, 11};
//    uint64_t logSize2 = uint64_t(std::floor(std::log2(EPSILON_SIZE)) - 5);
    approximateEntropyTest = { 1, 2, //logSize2 / 2, logSize2
                             4, 8};
}

}
