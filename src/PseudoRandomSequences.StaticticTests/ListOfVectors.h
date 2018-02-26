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
    class iterator;
public:
    ListOfVectors(size_t listSize, size_t vectorSize);

    size_t listSize() const { return listSize_; }
    size_t vectorSize() const { return vectorSize_; }

    T& operator[] (size_t index) { return container[index / vectorSize()][index % vectorSize()]; }
    const T& operator[] (size_t index) const { return container[index / vectorSize()][index % vectorSize()]; }


    //typename iterator begin();
    //iterator end() { return iterator(this, listSize() * vectorSize()); }

private:
    size_t listSize_;
    size_t vectorSize_;

    vector<vector<T> > container;
};

//template <class T>
//typename ListOfVectors<T>::iterator ListOfVectors<T>::begin() { return ListOfVectors<T>::iterator(); }

template <class T>
ListOfVectors<T>::ListOfVectors(size_t listSize, size_t vectorSize)
    : listSize_(listSize),
      vectorSize_(vectorSize),
      container(listSize, vector<T>(vectorSize))
{
}

template <class T>
class iterator
{
    friend ListOfVectors<T>;
    typedef typename vector<T>::iterator IteratorType;
    typedef ListOfVectors<T> * ListOfVectorsPtr;
public:
    iterator() {}

    iterator(ListOfVectorsPtr pList, size_t position) : pList_(pList), position_(position) {
        iter_ = pList->container[position / pList->vectorSize()].begin();
        std::advance(iter_, position % pList->vectorSize());
    }

    iterator & operator++() {

    }

    T& operator*() {
        return *iter_;
    }
private:
    IteratorType iter_;
    size_t position_;
    ListOfVectorsPtr pList_;
};

}

#endif // LISTOFVECTORS_H
