#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "point2d.h"

#ifdef QT_H
#include <QPoint>
#endif
#include <stdint.h>
#include <algorithm>

namespace LipaboyLib {

static const double epsilon = 1e-5;

class Vector3D
{
public:
    Vector3D() : X(0.0), Y(0.0), Z(0.0) {}
    Vector3D(double x1, double y1, double z1) : X(x1), Y(y1), Z(z1) {}
    Vector3D(Point2D point, double z1) : X(point.x()), Y(point.y()), Z(z1) {}
#ifdef QT_H
    Vector3D(QPoint point) : X(point.x()), Y(point.y()), Z(0.0) {}
#endif

    double x() const { return X; }
    double y() const { return Y; }
    double z() const { return Z; }
    double get(uint32_t index) const { return ((index == 0) ? x() : ((index == 1) ? y() : z())); }
    double& rx() { return X; }
    double& ry() { return Y; }
    double& rz() { return Z; }
    double& rget(uint32_t index) { return ((index == 0) ? rx() : ((index == 1) ? ry() : rz())); }

    Vector3D operator- () const { return Vector3D(-X, -Y, -Z); }
    Vector3D operator- (const Vector3D &obj) const { return Vector3D(X - obj.X, Y - obj.Y, Z - obj.Z); }
    Vector3D operator+ (const Vector3D &obj) const { return Vector3D(X + obj.X, Y + obj.Y, Z + obj.Z); }
    Vector3D operator* (double coef) const { return Vector3D(X * coef, Y * coef, Z * coef); }
    Vector3D operator* (const Vector3D& obj) const { return Vector3D(X * obj.X, Y * obj.Y, Z * obj.Z); }
    const Vector3D& operator*= (const Vector3D& obj) { return ((*this) = (*this) * obj); }
    const Vector3D& operator*= (double coef) { return ((*this) = (*this) * coef); }
    Vector3D opposite() const { return Vector3D(1.0 / X, 1.0 / Y, 1.0 / Z); }
    Point2D toPoint2D() const { return Point2D(X, Y); }
    double length() const { return std::sqrt(X * X + Y * Y + Z * Z); }
    Vector3D abs() const { return Vector3D(std::abs(X), std::abs(Y), std::abs(Z)); }
    double getMaxAbsCoord() const { return std::max(std::abs(X), std::max(std::abs(Y), std::abs(Z))); }

    bool compareWithZero() const { return (std::abs(X - 0.0) < epsilon) && (std::abs(Y - 0.0) < epsilon)
                && (std::abs(Z - 0.0) < epsilon); }
    bool operator== (const Vector3D& obj) const { return (std::abs(X - obj.X) < epsilon)
                && (std::abs(Y - obj.Y) < epsilon) && (std::abs(Z - obj.Z) < epsilon); }
    bool operator!= (const Vector3D& obj) const { return !(*this == obj); }

    const Vector3D& normalize() { return (*this) *= (1 / length()); }

private:
    double X;
    double Y;
    double Z;
};

inline double scalarMulti(const Vector3D& first, const Vector3D& second) {
    return first.x() * second.x() + first.y() * second.y() + first.z() * second.z();
}

//good
inline Vector3D vectorMulti(const Vector3D& first, const Vector3D& second) {
    return Vector3D(first.y() * second.z() - first.z() * second.y(),
                    - first.x() * second.z() + first.z() * second.x(),
                    first.x() * second.y() - first.y() * second.x());
}

inline Vector3D maxVector(const Vector3D& first, const Vector3D& second) {
    return Vector3D(std::max(first.x(), second.x()), std::max(first.y(), second.y()),
                    std::max(first.z(), second.z()));
}

}

#endif // VECTOR3D_H
