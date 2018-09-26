#ifndef VECTOR2D_H
#define VECTOR2D_H

#ifdef QT_H
#include <QPoint>
#endif
#include <cmath>
#include "point2d.h"

namespace LipaboyLib {

class Vector2D
{
public:
#ifdef QT_H
    Vector2D(QPoint point) : x(point.x()), y(point.y()) {}
#endif
	Vector2D(Point2D point) : x(point.x()), y(point.y()) {}

    double length() const { return std::sqrt(x * x + y * y); }
    double squaredLength() const { return (x * x + y * y); }

private:
    double x;
    double y;
};

}

#endif // VECTOR2D_H
