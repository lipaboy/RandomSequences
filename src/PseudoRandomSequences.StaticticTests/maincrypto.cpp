#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <functional>

#include "statTests/include/generators.h"

using namespace statistical_tests_space;

using std::cout;
using std::endl;

#include <boost/range/any_range.hpp>

using AnyRange = boost::any_range<int, boost::forward_traversal_tag, int, std::ptrdiff_t>;

int main(int argc, char *argv[]) {

    auto res = sha256("0");
    cout << res << endl;

    cout << "Tests the tests: ";
    cout << runUnitTests() << endl;
    cout << "Run tests: " << endl;
    return generatorsTestConfigRun(argc, argv);


//    int size = 100;
//    double average = 94.999 / size;
//    cout << isTestSuccessful(average
//                             + size * (MEANING_LEVEL - (size - 1.) / size + 1e-3) * (1. - average)) << endl;
//    double kek = (average + 1e-5) * MEANING_LEVEL / (1. - MEANING_LEVEL);
//    cout << isTestSuccessful(kek) << " " << kek << endl;

//using namespace std::placeholders;
//    using IntPtr = std::unique_ptr<int>;
//    using std::vector;
//    vector<IntPtr> first;
//    first.push_back(std::move(std::make_unique<int>(1)));
//    vector<IntPtr> second;
//    second.push_back(std::move(std::make_unique<int>(2)));
//    vector<IntPtr> result;
////    std::transform<typename std::vector<IntPtr>::iterator,
////                        typename vector<IntPtr>::iterator, typename std::function<IntPtr(IntPtr&)> >
////            (first.begin(), first.end(), std::back_inserter(result),
////                   [] (IntPtr& a) -> IntPtr {
////                       return std::move(a);
////                   });
//    std::transform(second.begin(), second.end(),
//                  std::transform(first.begin(), first.end(), std::back_inserter(result),
//                                 [] (IntPtr& a) {
//                                     return std::move(a);
//                                 }), [] (IntPtr& a) {
//                                        return std::move(a);
//    });

//    cout << *(result[0]) << *(result[1]) << endl;
}

