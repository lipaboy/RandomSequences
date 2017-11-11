#ifndef FIXED_PRECISION_NUMBER_H
#define FIXED_PRECISION_NUMBER_H

#include "../common_interfaces/comparable.h"
#include "../common_interfaces/either_comparable.h"
#include "../common_interfaces/algebra.h"

#include <ostream>

namespace LipaboyLib {

	template <typename T, typename IntegerType = int>
	inline constexpr T powDozen(IntegerType power) {
		return (power < static_cast<IntegerType>(0)) ? 
				((power > static_cast<IntegerType>(-1)) ? static_cast<T>(1) :
					static_cast<T>(0.1) * powDozen<T>(power + static_cast<IntegerType>(1)))
			: ((power < static_cast<IntegerType>(1)) ? static_cast<T>(1) : static_cast<T>(10)
				* powDozen<T>(power - static_cast<IntegerType>(1)));
	}

	template <typename T, typename IntegerPrecisionType, 
		IntegerPrecisionType fraction, IntegerPrecisionType dozenPower>
	class FixedPrecisionNumber : 
		public Comparable, 
		public EitherComparable<T>,
		public Algebra<T>,
		public NumberSettable<T>
	{
	public:
		explicit
			FixedPrecisionNumber(T _number = T()) noexcept
				: number(_number) {}

		bool operator<(const T& val) const noexcept { return (getNumber() < val
			- static_cast<T>(fraction * powDozen<T, IntegerPrecisionType>(dozenPower))); }
		bool operator<=(const T& val) const noexcept { return (getNumber() <= val
			+ static_cast<T>(fraction * powDozen<T, IntegerPrecisionType>(dozenPower))); }
		bool operator==(const T& val) const noexcept {
			return (getNumber() >= val
					- static_cast<T>(fraction * powDozen<T, IntegerPrecisionType>(dozenPower)))
				&& (getNumber() <= val
					+ static_cast<T>(fraction * powDozen<T, IntegerPrecisionType>(dozenPower)));
		}

		bool operator<(const Comparable& obj) const noexcept {
			return (*this) < dynamic_cast<const FixedPrecisionNumber&>(obj).getNumber(); }
		bool operator<=(const Comparable& obj) const noexcept {
			return (*this) <= dynamic_cast<const FixedPrecisionNumber&>(obj).getNumber(); }
		bool operator==(const Comparable& obj) const noexcept {
			return ((*this) == dynamic_cast<const FixedPrecisionNumber&>(obj).getNumber()); }

		void setNumber(T const & val) noexcept { number = val; }
		T const & getNumber() const noexcept { return number; }
		FixedPrecisionNumber const & operator= (T const & val) noexcept { setNumber(val); return *this; }

		operator T() noexcept { return number; }

		//friend std::ostream& operator<< (std::ostream& o, FixedPrecisionNumber const & number);
	private:
		T number;
	};

	/*template <typename T, typename IntegerPrecisionType,
		IntegerPrecisionType fraction, IntegerPrecisionType dozenPower>
	inline std::ostream& operator<< (std::ostream& o, 
			FixedPrecisionNumber<T, IntegerPrecisionType, fraction, dozenPower> const & number) {
		return o << number.getNumber();
	}*/

}

#endif //FIXED_PRECISION_NUMBER_H
