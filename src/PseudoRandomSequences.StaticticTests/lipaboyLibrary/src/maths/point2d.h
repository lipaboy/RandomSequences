#ifndef POINT2D_H
#define POINT2D_H

#ifdef QT_H
#include <QPoint>
#include <QPointF>
#endif
#include <cmath>

namespace LipaboyLib {

	class Point2D
	{
	public:
		Point2D() {}
		Point2D(double x, double y) : X(x), Y(y) {}
#ifdef QT_H
		Point2D(QPointF point) : X(point.x()), Y(point.y()) {}
		Point2D(QPoint point) : X(point.x()), Y(point.y()) {}
#endif

		double x() const { return X; }
		double y() const { return Y; }
		double& rx() { return X; }
		double& ry() { return Y; }

		Point2D operator+ (const Point2D& obj) const { return Point2D(X + obj.X, Y + obj.Y); }
		Point2D operator* (const Point2D& obj) const { return Point2D(X * obj.X, Y * obj.Y); }

#ifdef QT_H
		QPoint&& toQPoint() const {
			return std::move(QPoint(static_cast<int>(std::round(X)),
				static_cast<int>(std::round(Y))));
		}
#endif

		bool operator==(const Point2D& obj) const { return (std::abs(X - obj.X) < 1e-5 && std::abs(Y - obj.Y) < 1e-5); }
		bool operator!=(const Point2D& obj) const { return !((*this) == obj); }

	private:
		double X;
		double Y;
	};

}

#endif // POINT2D_H
