#ifndef INTERVAL_H
#define INTERVAL_H

#include "../common_interfaces/containable.h"
#include "../common_interfaces/either_comparable.h"

#include <functional>

namespace LipaboyLib {

	//TODO: write comparison with Infinity (Infinity == Infinity)

	template <class T>
	class Infinity : public EitherComparable<T> {};

	template <class T>
	class PositiveInfinity : public Infinity<T> {
	public:
		bool operator< (const T& number) const { return false; }
		bool operator<= (const T& number) const { return false; }
		bool operator== (const T& number) const { return false; }
	};

	template <class T>
	class NegativeInfinity : public Infinity<T> {
	public:
		bool operator< (const T& number) const { return true; }
		bool operator<= (const T& number) const { return true; }
		bool operator== (const T& number) const { return true; }
	};
	
	//TODO: Add debug function to output result ( if contains then "c in [a, b]" or "c out [a, b]" )
	//TODO: add output to ostream

	template <class T, typename LeftComparison, typename RightComparison>
	class Interval : public Containable<T>
	{
	public:
		Interval(const T& _leftBorder = T(), const T& _rightBorder = T())
			: leftBorder(_leftBorder), rightBorder(_rightBorder) {}
		bool in(const T& element) const { return contains(element); }
		bool out(const T& element) const { return !in(element); }
		//TODO: think about advantages of these methods. If you include them then will override these ones into PositiveRay
		bool outLeft(const T& element) const { return !isLeftCompare(element); }
		bool outRight(const T& element) const { return !isLeftCompare(element); }

		bool contains(const T& element) const {
			return isLeftCompare(element) && isRightCompare(element);
		}

		const T& left() const { return leftBorder; }
		const T& right() const {return rightBorder; }
		T& rLeft() { return leftBorder; }
		T& rRight() { return rightBorder; }

		bool operator==(const Interval& other) const {
			return (leftBorder == other.leftBorder)
				&& (rightBorder == other.rightBorder);
		}
		bool operator!=(const Interval& other) const { return !((*this) == other); }

		//You can't write method length() because your class Interval will become less general
		//Solution: but you can inherit from Interval class NumericInterval where add this method (and maybe Infinity)

	protected:
		bool isLeftCompare(const T& element) const { return leftComp(leftBorder, element); }
		bool isRightCompare(const T& element) const { return rightComp(element, rightBorder); }
	private:
		T leftBorder;
		T rightBorder;
		LeftComparison leftComp;
		RightComparison rightComp;
	};

	template <class T>
	using OpenInterval = Interval<T, std::less<T>, std::less<T> >;

	template <class T>
	using CloseInterval = Interval<T, std::less_equal<T>, std::less_equal<T> >;

	//template <class T>
	//class Ray;
	//
	//template <class T, class LeftComparison>
	//class PositiveRay : public Interval<T, LeftComparison, std::less<> > {
	//public:
	//	PositiveRay(const T& _leftBorder = T()) : Interval(_leftBorder) {}
	//	virtual bool contains(const T& element) const { return isLeftCompare(element); }
	//	//TODO: how I can return Infinity from method right()???
	//};


	//use * to function
	/*std::function<bool(const T& lhs, const T& element)> leftComparison,
	std::function<bool(const T& element, const T& rhs)> rightComparison*/
}

#endif // INTERVAL_H

