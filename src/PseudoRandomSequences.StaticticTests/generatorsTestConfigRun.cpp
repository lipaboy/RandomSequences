#include "pseudoRandomSequences.h"

#include <cstring>

#include <omp.h>

#include <algorithm>
#include <iterator>
#include <bitset>
#include <string.h>
#include <array>
#include <cmath>
#include <random>

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>

#include <memory>
//#include <boost/lexical_cast.hpp>
#ifdef __linux__
#include <openssl/sha.h>
#endif

#include "lipaboyLibrary/src/maths/fixed_precision_number.h"
#include "statTests/include/generators.h"

namespace statistical_tests_space {

using namespace std::chrono;

using std::string;
using std::vector;
using std::cout;
using std::endl;

//TODO: try to use GoogleTests

namespace {
	class MyBoolRange {
	private:
		BoolIterator _begin, _end;
	public:
		MyBoolRange(BoolIterator beg, BoolIterator end) : _begin(beg), _end(end) {}
		BoolIterator const & begin() { return _begin; }
		BoolIterator const & end() { return _end; }
	};
}


int generatorsTestConfigRun(int argc, char * argv[]) {
	time_t t;
    std::srand(static_cast<unsigned int>(std::time(&t)));

    //-----------------------------Input data-----------------------------//

    int sizesStartIndex = 4;
    size_t len = (argc > 1) ? strlen(argv[1]) : 0;
    size_t countSizes = (argc < sizesStartIndex + 1)
            ? 0 : std::stoul(argv[sizesStartIndex]);
    if (countSizes <= 0 || argc < sizesStartIndex + 1 + int(countSizes) || len < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
//            << "input possibility, "
            << "traversal count, generator name, count of seq. sizes (N = "
            << countSizes << "), size 1, size 2, .., size N)"
			<< endl;
		return -1;
	}
    int inputOppositePossibility = 2;   //static_cast<int>(
        //std::round(1.0 / boost::lexical_cast<double>(argv[2]))
    //);

   // std::vector<string> generatorNames { argv[4] };
    const size_t TRAVERSAL_COUNT_SMALL = std::stoul(argv[2]);
    string testKey(argv[1]);
    vector<size_t> seqSizes;
    for (int i = sizesStartIndex + 1; i < argc; i++)
        seqSizes.push_back(std::stoul(argv[i]));


    // TODO: add test performance for conversation(?) and tests

    // TODO: bad computation of input possibility (use another input format) (bad because possibility=2/3 won't be work)
    // Kbits

	std::minstd_rand generatorMinstdRand;
    std::minstd_rand0 generatorMinstdRand0;
	std::knuth_b generatorKnuthB;
    std::ranlux24 generatorRanlux24;
	std::ranlux48 generatorRanlux48;	//failure with normal_distribution and with chi_squared_distribution
	std::random_device generatorRandomDevice;
    std::default_random_engine generatorDefaultRandomEngine;
    std::mt19937_64 generatorMt19937_64;

    auto wholeTimeExpend = getCurrentClockTime();
//	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
        std::string genName = argv[3];

		//--------------------Container---------------------//

        uint32_t atom = 1u;
               // 1000u;
                //1024u;

        const size_t TRAVERSAL_COUNT_LARGE = TRAVERSAL_COUNT_SMALL;
//        const size_t TRAVERSAL_THRESHOLD = size_t(1e5);
        Sequence epsilon;
        // TODO: Remove this crutch
		tp.n = 0;
        tp.numOfBitStreams = static_cast<int>(TRAVERSAL_COUNT_LARGE);
        tp.n = seqSizes[0] * atom;
        const size_t seqSize = tp.n * size_t(tp.numOfBitStreams);
//        for (auto jSize : seqSizes) {
//			tp.n += jSize * atom * ((jSize < TRAVERSAL_THRESHOLD) ? TRAVERSAL_COUNT_LARGE
//				: TRAVERSAL_COUNT_SMALL);
//		}

        auto genTimeExpend = getCurrentClockTime();
        std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
        //std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0) it is logical (you need 3.5 instead of 3.0 because even/odd-ness)

        char genNameStr[80];
        strcpy(genNameStr, genName.c_str());
        char * inputFilename = strtok(genNameStr, "=");
        genName = inputFilename;

        // ------------------------- Output Results ----------------------------- //
        std::ofstream resFile;
        string outFilename = "resStdGenerators_" + genName;
        for (auto iSize : seqSizes)
            outFilename += "_" + std::to_string(iSize);
        resFile.open(outFilename + ".dat", std::ios::out | std::ios::trunc);
        cout << outFilename << endl;

        if ("file" == genName) {
            // TODO: add checking the data of input
            char * isBitRead = strtok(nullptr, "=");
            inputFilename = strtok(nullptr, "=");
            if (strcmp(isBitRead, "2") == 0)
                epsilon = readSequenceByBitFromFile(inputFilename, seqSize);
            else if (strcmp(isBitRead, "special") == 0)
                epsilon = readSequenceByByteFromFile(inputFilename, seqSize, '0', true);
            else
                epsilon = readSequenceByByteFromFile(inputFilename, seqSize, '0', false);
        }
        else if ("minstd_rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMinstdRand, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorMinstdRand))) % inputOppositePossibility == 0);
                });
        }
        else if ("minstd_rand0" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMinstdRand0, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorMinstdRand0))) % inputOppositePossibility == 0);
                });
        }
        else if ("knuth_b" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorKnuthB, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorKnuthB))) % inputOppositePossibility == 0);
                });
        }
        else if ("ranlux24" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRanlux24, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRanlux24))) % inputOppositePossibility == 0);
            });
        }
        else if ("ranlux48" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRanlux48, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorRanlux48))) % inputOppositePossibility == 0);
                });
        }
        else if ("random_device" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRandomDevice, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorRandomDevice))) % inputOppositePossibility == 0);
                });
        }
        else if ("rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                []() -> bool {
                    return ((std::rand() % 2) == 0);
                });
        }
        else if ("mersenne" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMt19937_64, &distribution]() -> bool {
                    return (int(std::round(distribution(generatorMt19937_64))) % inputOppositePossibility == 0);
                });
        }
        else if ("bad" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                []() -> bool {
                    static int i = 0;
                    return (i++ < int(tp.n));
                });
        }
        else if ("lcg" == genName)
			epsilon = lcg();
        else if ("SHA1" == genName) { //wrong given results
            epsilon = SHA1();
        }
        else if ("modExp" == genName)       //slow
			epsilon = modExp();
        else if ("bbs" == genName)          //slow
            epsilon = bbs();
		else if ("exclusiveOR" == genName)
			epsilon = exclusiveOR();
        else if ("micali_schnorr" == genName)
            epsilon = micali_schnorr();
        else if ("cubicRes" == genName)
            epsilon = cubicRes();
        else if ("quadRes1" == genName)
            epsilon = quadRes1();
        else if ("quadRes2" == genName)
            epsilon = quadRes2();
        else {
            cout << "Error: no such generator" << endl;
            return -1;
        }

        {
            cout << "Generator time expend: "
                 << getTimeDifferenceInMillis(genTimeExpend, getCurrentClockTime()) / 1000
                 << " secs." << endl;

//            // sha-256
//            Sequence epsilonSha(epsilon.size());
//            string str = "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592";
//            constexpr size_t DIGEST_SIZE = 64;
//            for (size_t i = 0; i < epsilon.size(); ) {
//                str[i % DIGEST_SIZE] = '0' + epsilon[i];
//                if ((++i) % DIGEST_SIZE == 0) {
//                    auto shaStr = sha256(str);
//                    for (size_t j = 0; j < DIGEST_SIZE; j++) {
//                        // DANGER: unsigned arithmetic but must work
//                        epsilonSha[i - DIGEST_SIZE + j] = (shaStr[j] % 2 == 0);
//                    }
//                }
//            }
//            epsilon = std::move(epsilonSha);
        }

		size_t accumulatorSize = 0u;
        for (auto & iSize : seqSizes) {
			size_t inputSize = //1024u 
				atom * iSize;
			
			cout << ">-----------------------New Size-----------------------<" << endl;
			cout << endl << genName << endl;
			cout << "Sequence size = " << inputSize << endl;
            //-------------Input----------------//

            vector<string> testNames =
                    getStatisticTestNames(testKey, inputSize);
            vector<double> testResults(testNames.size());

            size_t traversalCount = TRAVERSAL_COUNT_LARGE;

// TODO: create define for DEBUG to remove omp parallelism
#pragma omp parallel for shared(genName, traversalCount, testKey, testResults)
            for (int jTraver = 0; static_cast<size_t>(jTraver) < traversalCount; jTraver++)
            {
                {
                    if (jTraver % 10 == 0)
                        cout << "(INFO) Traversal: " << jTraver << endl;
                }
				// Generator factory
                MyBoolRange epsilonRange(epsilon.begin(), epsilon.end());
                {
                    {
                        auto iterBegin = epsilon.begin();
                        auto iterEnd = epsilon.begin();
                        #pragma omp critical
                        {
                            std::advance(iterBegin, accumulatorSize);
                            std::advance(iterEnd, accumulatorSize + inputSize);
                            accumulatorSize += inputSize;
                        }
                        epsilonRange = MyBoolRange(iterBegin, iterEnd);
					}
				}

                // logging
                {
                    if (jTraver % 10 == 0)
                    {
                        auto iterEnd = epsilonRange.begin();
                        std::advance(iterEnd, 20);
                        std::copy(epsilonRange.begin(), iterEnd,
                            std::ostream_iterator<bool>(cout, ""));
                        std::cout << std::endl;
                    }
                }

				//----------------Tests-----------------//
				{
                    auto currResults = runStatisticalTests(epsilonRange.begin(), epsilonRange.end(), testKey,
                                        std::make_shared<TestParameters>());
                    #pragma omp critical
                    {
                        std::transform(currResults.begin(), currResults.end(), //first source
                            testResults.begin(),                                //second source
                            testResults.begin(),                                //destination
                            [](IStatisticalTest::TestResultType value, double count) -> double {
                                return ((value ==
                                         IStatisticalTest::TestResultType::CANCELLED)
                                        ? (count - 1000.)
                                        : (value ==
                                           IStatisticalTest::TestResultType::FAILURE)
                                          ? (1. + count) : count);
                        });
                    }
                }
			}

			//----------------Write results-----------------//
            {
                if (&iSize == &seqSizes.front()) {
                    resFile << "\t";
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
                    resFile << endl;
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(cout, "\t"));
                    cout << endl;
                }
                resFile << iSize << "_Kbits\t";
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
                resFile << endl;
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(cout, "\t"));
                cout << endl;
            }

            resFile.flush();
			testNames.clear();
		}
        resFile.close();
//	}

    cout << endl << "Whole time expend: "
         << getTimeDifferenceInMillis(wholeTimeExpend, getCurrentClockTime()) / 1000
         << "sec." << endl;
	

	return 0;
}

//-----------------Extra functions---------------------//

Sequence readSequenceByBitFromFile(string const & inputFile, size_t sequenceSize) {
    using std::ifstream;
    typedef unsigned char BlockReadType;

    Sequence epsilon(sequenceSize);
    try {
        ifstream inFile;

        inFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        inFile.open(inputFile, std::ios::in);

        BlockReadType buffer;
        for (size_t i = 0; i < epsilon.size() / 8; i++) {
            char symbol;
            inFile >> symbol;
            buffer = static_cast<BlockReadType>(symbol);
            for (size_t bit = 0; bit < 8 * sizeof(BlockReadType); bit++) {
                epsilon[i * 8 + bit] = static_cast<BitSequence>((buffer & (1 << bit)) >> bit);
            }
        }
//        std::copy_n(epsilon.begin(), 16, std::ostream_iterator<BitSequence>(cout));
//        cout << endl;

        inFile.close();
    } catch(ifstream::failure e) {
        std::cerr << "Exception opening/reading/closing file " << inputFile << endl;
    }

    return epsilon;
}

Sequence readSequenceByByteFromFile(string const & inputFile, size_t sequenceSize,
                                           char isZero, bool isSpecialFormat) {
    // TODO: write catching exceptions
    using std::ifstream;

    Sequence epsilon(sequenceSize);
    try {
        ifstream inFile;
        inFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        inFile.open(inputFile, std::ios::in);

        if (isSpecialFormat) {
            for (int i = 0; i < static_cast<int>(epsilon.size()); i++) {
                char symbol;
                inFile >> symbol;
                epsilon[i] = ((symbol & 1) != 0)     //for PI number file if hex format
                                ? 0 : 1;
            }
        }
        else {
            for (int i = 0; i < static_cast<int>(epsilon.size()); i++) {
                char symbol;
                inFile >> symbol;
                epsilon[i] = (symbol != isZero);      //for others
            }
        }
        inFile.close();
    } catch(ifstream::failure e) {
        std::cerr << "Exception opening/reading/closing file " << inputFile << endl;
    }

    return std::move(epsilon);
}

const unsigned int SHA256::sha256_k[64] = //UL = uint32
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256::transform(const unsigned char *message, unsigned int block_nb)
{
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    int i;
    int j;
    for (i = 0; i < (int) block_nb; i++) {
        sub_block = message + (i << 6);
        for (j = 0; j < 16; j++) {
            SHA2_PACK32(&sub_block[j << 2], &w[j]);
        }
        for (j = 16; j < 64; j++) {
            w[j] =  SHA256_F4(w[j -  2]) + w[j -  7] + SHA256_F3(w[j - 15]) + w[j - 16];
        }
        for (j = 0; j < 8; j++) {
            wv[j] = m_h[j];
        }
        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
                + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }
        for (j = 0; j < 8; j++) {
            m_h[j] += wv[j];
        }
    }
}

void SHA256::init()
{
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = SHA224_256_BLOCK_SIZE - m_len;
    rem_len = len < tmp_len ? len : tmp_len;
    memcpy(&m_block[m_len], message, rem_len);
    if (m_len + len < SHA224_256_BLOCK_SIZE) {
        m_len += len;
        return;
    }
    new_len = len - rem_len;
    block_nb = new_len / SHA224_256_BLOCK_SIZE;
    shifted_message = message + rem_len;
    transform(m_block, 1);
    transform(shifted_message, block_nb);
    rem_len = new_len % SHA224_256_BLOCK_SIZE;
    memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
    m_len = rem_len;
    m_tot_len += (block_nb + 1) << 6;
}

void SHA256::final(unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;
    block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9)
                     < (m_len % SHA224_256_BLOCK_SIZE)));
    len_b = (m_tot_len + m_len) << 3;
    pm_len = block_nb << 6;
    memset(m_block + m_len, 0, pm_len - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(len_b, m_block + pm_len - 4);
    transform(m_block, block_nb);
    for (i = 0 ; i < 8; i++) {
        SHA2_UNPACK32(m_h[i], &digest[i << 2]);
    }
}

std::string sha256(std::string input)
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest,0,SHA256::DIGEST_SIZE);

    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update( reinterpret_cast<const unsigned char*>(input.c_str()), input.length());
    ctx.final(digest);

    char buf[2*SHA256::DIGEST_SIZE+1];
    buf[2*SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < int(SHA256::DIGEST_SIZE); i++)
        sprintf(buf+i*2, "%02x", digest[i]);
    return std::string(buf);
}

}


