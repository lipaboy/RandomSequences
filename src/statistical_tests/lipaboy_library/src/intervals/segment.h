#ifndef SEGMENT_H
#define SEGMENT_H

#include <cmath>

//#include "interval.h"

//#include <QDebug>

namespace LipaboyLib {

//TODO: you can add some functionality like :
// push in "stack" the results of comparation, for example:
//segment.add(x).add(y).getResult() - return bool
//segment.add(x) - return const Segment& to reuse it
// Likely solution: read about Args&&..

//common methods for Interface Plenty (in(), out())

template <class T>
class Segment
{
public:
    Segment() : leftBorder(static_cast<T>(0)), rightBorder(static_cast<T>(0)) {}
    Segment(T left1, T right1) : leftBorder(left1), rightBorder(right1) {}
    bool in(T element) const { return ((element >= leftBorder) && (element <= rightBorder)); }
    bool out(T element) const { return !in(element); }
    bool outLeft(T element) const { return (element < leftBorder); }
    bool outRight(T element) const { return (element > rightBorder); }

    T left() const { return leftBorder; }
    T right() const { return rightBorder; }
    T length() const { return rightBorder - leftBorder; }

    void swap() { std::swap(leftBorder, rightBorder); }

protected:
    T leftBorder;
    T rightBorder;
};

template <>
class Segment<double>
{
public:
    Segment(double left1, double right1, double precision = 1e-6) : leftBorder(left1), rightBorder(right1),
        epsilon(std::abs(precision)) {}

    bool in(double element) const { return ((element > leftBorder - epsilon)
                                            && (element < rightBorder + epsilon)); }
    bool out(double element) const { return !in(element); }
    bool outLeft(double element) const { return (element <= leftBorder - epsilon); }
    bool outRight(double element) const { return (element >= rightBorder + epsilon); }

    double left() const { return leftBorder; }
    double right() const { return rightBorder; }

    void swap() { std::swap(leftBorder, rightBorder); }

protected:
    double leftBorder;
    double rightBorder;
    double epsilon;
};



}

#endif // SEGMENT_H
