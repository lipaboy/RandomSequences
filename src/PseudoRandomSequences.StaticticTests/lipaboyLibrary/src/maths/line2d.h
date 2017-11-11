#ifndef LINE2D_H
#define LINE2D_H

#include "point2d.h"

namespace LipaboyLib {

	class Line2D
	{
	public:
		Line2D() {}
		Line2D(const Point2D& p1, const Point2D& p2) : First(p1), Second(p2) {}
		Line2D(double x1, double y1, double x2, double y2) : First(x1, y1), Second(x2, y2) {}

		const Point2D& first() const { return First; }
		const Point2D& second() const { return Second; }
		Point2D& rfirst() { return First; }
		Point2D& rsecond() { return Second; }

	private:
		Point2D First;
		Point2D Second;
	};
}

#endif // LINE2D_H
