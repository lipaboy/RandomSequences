#ifndef ACCURACY_NUMBER_H
#define ACCURACY_NUMBER_H

#include "../common_interfaces/comparable.h"
#include "../common_interfaces/either_comparable.h"
#include "../common_interfaces/algebra.h"

#include <ostream>

namespace LipaboyLib {

	//TODO: add set epsilon

	template <class T>
	class PrecisionNumber : 
		public EitherComparable<T>, 
		public Comparable,
		public Algebra<T>,
		public NumberSettable<T>
	{
	public:
		explicit
		PrecisionNumber(const T& _number = T(), const T& _precision = T()) noexcept
			: number(_number), epsilon(_precision) {}

		bool operator<(const T& val) const noexcept { return (getNumber() < val - epsilon); }
		bool operator<=(const T& val) const noexcept { return (getNumber() <= val + epsilon); }
		bool operator==(const T& val) const noexcept { 
			return (getNumber() >= val - epsilon) && (getNumber() <= val + epsilon);
		}

		bool operator<(const Comparable& obj) const noexcept { 
			return (*this) < dynamic_cast<const PrecisionNumber&>(obj).getNumber(); }
		bool operator<=(const Comparable& obj) const noexcept { 
			return (*this) <= dynamic_cast<const PrecisionNumber&>(obj).getNumber();}
		bool operator==(const Comparable& obj) const noexcept { 
			return ((*this) == dynamic_cast<const PrecisionNumber&>(obj).getNumber()); }

		void setNumber(T const & val) noexcept { number = val; }
		T const & getNumber() const noexcept { return number; }
		PrecisionNumber const & operator= (T const & val) noexcept { setNumber(val); return *this; }

		operator T() noexcept { return getNumber(); }

		//friend std::ostream& operator<< (std::ostream& o, PrecisionNumber<T> const & number);
	private:
		T epsilon;		//our precision
		T number;
	};

	/*template <class T>
	inline std::ostream& operator<< (std::ostream& o, PrecisionNumber<T> const & number) { 
		return o << number.getNumber(); 
	}*/

	typedef PrecisionNumber<double> PrecisionDouble;

}



//template <double _precision>
//using ConstAccuracyDouble = AccuracyFixedNumber<double, _precision>;

#endif	//ACCURACY_NUMBER_H
