#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <vector>

#include "vector3d.h"
#include "point2d.h"

namespace LipaboyLib {


class Vector4D
{
protected:
    std::vector<double> container = std::vector<double>(4);

public:
    Vector4D() {}
    Vector4D(double vect1[4]);
    Vector4D(double v1, double v2, double v3, double v4);
    Vector4D(Point2D point, double v3, double v4) : Vector4D(point.x(), point.y(), v3, v4) {}
    Vector4D(Vector3D vec, double v4) : Vector4D(vec.x(), vec.y(), vec.z(), v4) {}

    double& operator[] (uint32_t index) { return container[index]; }
    double operator[] (uint32_t index) const { return container[index]; }
    double get(uint32_t index) const { return container[index]; }
    Vector3D toVector3D() const { return Vector3D(container[0], container[1], container[2]); }

    Vector4D operator* (const double coef) const;
    //divide on W coordinate (not casting to unitary length)
    const Vector4D& normalize() { (*this) = (*this) * (1.0 / container[3]); return *this; }
    double sumElements() const { return get(0) + get(1) + get(2) + get(3); }

    //Vector4D operator* (const Vector4D& obj) const;
};

//class VectorColumn4D;

class VectorRow4D : public Vector4D
{
public:
    VectorRow4D() : Vector4D() {}
    VectorRow4D(const Vector4D& obj) : Vector4D(obj) {}
    VectorRow4D(double v1, double v2, double v3, double v4) : Vector4D(v1, v2, v3, v4) {}
    VectorRow4D(Vector3D vec, double v4) : Vector4D(vec, v4) {}
};

class VectorColumn4D : public Vector4D
{
public:
    VectorColumn4D() : Vector4D() {}
    VectorColumn4D(double container[4]) : Vector4D(container) {}
    VectorColumn4D(const Vector4D& obj) : Vector4D(obj) {}
    VectorColumn4D(double v1, double v2, double v3, double v4) : Vector4D(v1, v2, v3, v4) {}
    VectorColumn4D(Vector3D vec, double v4) : Vector4D(vec, v4) {}
    VectorColumn4D(Point2D point, double v3, double v4) : Vector4D(point, v3, v4) {}
};

inline VectorColumn4D transposeVec(const VectorRow4D& obj) { return VectorColumn4D(obj); }
inline VectorRow4D transposeVec(const VectorColumn4D& obj) { return VectorRow4D(obj); }

double operator* (const VectorRow4D& v1, const VectorColumn4D& v2);



}

#endif // VECTOR4D_H
