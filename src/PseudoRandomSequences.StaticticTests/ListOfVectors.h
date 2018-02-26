#ifndef LISTOFVECTORS_H
#define LISTOFVECTORS_H

#include <vector>

namespace LipaboyLib {

using std::size_t;
using std::vector;

template <class T>
class ListOfVectors
{
public:
    ListOfVectors(size_t listSize, size_t vectorSize);

    size_t listSize() const { return listSize_; }
    size_t vectorSize() const { return vectorSize_; }

    T& operator[] (size_t index) { return container[index / vectorSize()][index % vectorSize()]; }
    const T& operator[] (size_t index) const { return container[index / vectorSize()][index % vectorSize()]; }

private:
    size_t listSize_;
    size_t vectorSize_;

    vector<vector<T> > container;
};

template <class T>
ListOfVectors<T>::ListOfVectors(size_t listSize, size_t vectorSize)
    : listSize_(listSize),
      vectorSize_(vectorSize),
      container(listSize, vector<T>(vectorSize))
{
}

}

#endif // LISTOFVECTORS_H
