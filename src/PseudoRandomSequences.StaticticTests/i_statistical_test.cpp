#include "i_statistical_test.h"

namespace statistical_tests_space {

IStatisticalTest::ReturnValueType
BlockFrequencyTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.blockFrequencyTest)
        container.push_back(BlockFrequency(param, size, sequenceIter));
    return std::move(container);
}

IStatisticalTest::ReturnValueType
NonOverlappingTemplateMatchingsTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.nonOverlappingTemplateMatchingsTest) {
        std::vector<double> temp = NonOverlappingTemplateMatchings(param, size, sequenceIter);
        double average = 0.;
        for (auto & elem : temp) {
            average += isTestSuccessful(elem);
        }
        const size_t size = temp.size();
        average /= size;
        // TODO: very complex expression. Need to simplify it
        container.push_back(size == 0 ? -1.
            : average + size * (MEANING_LEVEL - (size - 1.) / size + 1e-3) * (1. - average));
    }
    return std::move(container);
}

IStatisticalTest::ReturnValueType
OverlappingTemplateMatchingsTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.overlappingTemplateMatchingsTest)
        container.push_back(OverlappingTemplateMatchings(param, size, sequenceIter));
    return std::move(container);
}

IStatisticalTest::ReturnValueType
LinearComplexityTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.linearComplexityTest)
        container.push_back(LinearComplexity(param, size, sequenceIter));
    return std::move(container);
}

IStatisticalTest::ReturnValueType
SerialTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.serialTest) {
        auto res = Serial(param, size, sequenceIter);
        container.push_back(res.first);
        container.push_back(res.second);
    }
    return std::move(container);
}

IStatisticalTest::ReturnValueType
ApproximateEntropyTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    TestParameters testParameters(size);
    for (auto & param : testParameters.approximateEntropyTest) {
        // (M + 1) - bit block is used to compare
        container.push_back(ApproximateEntropy(param, size, sequenceIter));
    }
    return std::move(container);
}

IStatisticalTest::ReturnValueType
CumulativeSumsTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    auto res = CumulativeSums(size, sequenceIter);
    container.push_back(res.first);
    container.push_back(res.second);
    return std::move(container);
}

IStatisticalTest::ReturnValueType
RandomExcursionsTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    auto result = RandomExcursions(size, sequenceIter);
    double average = 0.;
    for (auto & elem : result) {
        average += isTestSuccessful(elem);
    }
    size_t resSize = result.size();
    average /= resSize;
    // TODO: very complex expression. Need to simplify it
    container.push_back(resSize == 0 ? -1.
        : average + resSize * (MEANING_LEVEL - (resSize - 1.) / resSize + 1e-3) * (1. - average));
    return std::move(container);
}

IStatisticalTest::ReturnValueType
RandomExcursionsVariantTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;
    auto result = RandomExcursionsVariant(size, sequenceIter);
    double average = 0.;
    for (auto & elem : result) {
        average += isTestSuccessful(elem);
    }
    size_t resSize = result.size();
    average /= resSize;
    // TODO: very complex expression. Need to simplify it
    container.push_back(resSize == 0 ? -1.
        : average + resSize * (MEANING_LEVEL - (resSize - 1.) / resSize + 1e-3) * (1. - average));
    return std::move(container);
}

IStatisticalTest::ReturnValueType
BookStackTest::test(BoolIterator sequenceIter, size_type size) {
    ReturnValueType container;

    string inputFile = "bookStackInput" + std::to_string(omp_get_thread_num())
             + "_" + std::to_string(size) + ".dat";
    {
        typedef u_char BlockReadType;   //when was char instead of u_char nothing to change

        std::ofstream outFile;
        outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
        outFile.open(inputFile, std::ios::trunc);
        auto outIter = std::ostream_iterator<BlockReadType>(outFile);
        int bitPos = 0;
        char buffer = 0;
        auto iter = sequenceIter;
        for (size_t i = 0; i < size; i++) {
            buffer |= (*(iter++)) << (bitPos++);
            if (bitPos >= 8) {
                bitPos = 0;
                *(outIter++) = buffer;
                buffer = 0;
            }
        }
        if (bitPos > 0)
            *(outIter) = buffer;
        outFile.close();
    }

    TestParameters testParameters(size);
    // ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)
    for (auto param : testParameters.bookStackTest) {
        string sizeStr = std::to_string(size);
        string dimStr = std::to_string(param.dimension);
        string upperPartStr = std::to_string(param.upperPart);
        string filename = inputFile;
        std::vector<const char *> arguments{ "bs.exe",
            "-f", filename.c_str(),
            "-n", sizeStr.c_str(),	// file size (in bits)
            "-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
            //"-b", "0",				// blank between words
            "-u", upperPartStr.c_str()				// size of upper part book stack
        };
        if (param.upperPart > (1LL << 28))
            continue;
        container.push_back(bookStackTestMain(static_cast<int>(arguments.size()), &arguments[0]));
    }
    std::remove(inputFile.c_str());

    return std::move(container);
}


//----------------------Test parameters----------------------//

TestParameters::TestParameters(size_t EPSILON_SIZE)
    : blockFrequencyTest({ 2, //EPSILON_SIZE / 4, EPSILON_SIZE / 2,
                         //16, 32, 128
                         10
                         }),
      nonOverlappingTemplateMatchingsTest({ 3, 5, 7, //14
                                          }),                    //slow test!!!!
      overlappingTemplateMatchingsTest({ 2, 6, 12 })
{
    EPSILON_SIZE++;
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
