#ifndef CUT_OFF_BORDERS_H
#define CUT_OFF_BORDERS_H

//#include <interval.h>

namespace LipaboyLib {

	template <typename T>
	inline const T& cutOffBorder(const T& value, const T& rightBorder, const T& leftBorder) {
		return ((value < leftBorder) ? leftBorder : ((value > rightBorder) ? rightBorder : value));
	}

	template <typename T>
	inline const T& cutOffRightBorder(const T& value, const T& rightBorder) {
		return ((value > rightBorder) ? rightBorder : value);
	}

	template <typename T>
	inline const T& cutOffLeftBorder(const T& value, const T& leftBorder) {
		return ((value < leftBorder) ? leftBorder : value);
	}

	/*template <typename T>
	T cutOffBorder(T value, const CloseInterval<T>& border) {
		return ((border.outLeft(value)) ? border.left() : ((border.outRight(value)) ? border.right() : value));
	}*/

}

#endif //CUT_OFF_BORDERS_H
