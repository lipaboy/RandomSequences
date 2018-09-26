#ifndef CUBESEGMENT_H
#define CUBESEGMENT_H

#include "segment.h"
#include "../maths/vector3d.h"
//
//using namespace LipaboyLib;

namespace LipaboyLib {

class CubeSegment
{
public:
    CubeSegment(Segment<double> X, Segment<double> Y, Segment<double> Z) : x(X), y(Y), z(Z) {}
    bool in(LipaboyLib::Vector3D vec) const { return (x.in(vec.x()) && y.in(vec.y()) && z.in(vec.z())); }
    bool out(LipaboyLib::Vector3D vec) const { return !in(vec); }

    Segment<double> xSegment() const { return x; }
    Segment<double> ySegment() const { return y; }
    Segment<double> zSegment() const { return z; }

protected:
    Segment<double> x;
    Segment<double> y;
    Segment<double> z;
};

}

#endif // CUBESEGMENT_H
