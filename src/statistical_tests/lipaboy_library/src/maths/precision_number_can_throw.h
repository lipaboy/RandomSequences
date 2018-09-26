#ifndef ACCURACY_NUMBER_CAN_THROW_H
#define ACCURACY_NUMBER_CAN_THROW_H

#include "../common_interfaces/comparable.h"
#include "../common_interfaces/either_comparable.h"

#include <ostream>

namespace LipaboyLib {

	//That class distinguishes from PrecisionNumber at lack of noexcept

	template <class T>
	class ElemableCanThrow {
	public:
		ElemableCanThrow(const T& val) : elem(val) {}
		ElemableCanThrow const & operator=(T const & val) { set(val); return *this; }
		void set(const T& val) { elem = val; }		//I can't mark method as noexcept because I don't sure that operator= won't throw exception
		const T& get() const noexcept { return elem; }
	private:
		T elem;
	};

	template <class T>
	class EitherSummableCanThrow : public ElemableCanThrow<T> {
	public:
		EitherSummableCanThrow(const T& val) : ElemableCanThrow(val) { }
		T operator+(const EitherSummableCanThrow& other) const { return get() + other.get(); }
	};
	template <class T>
	T operator+(const EitherSummableCanThrow<T>& obj, const T& val) { return obj.get() + val; }
	template <class T>
	T operator+(const T& val, const EitherSummableCanThrow<T>& obj) { return val + obj.get(); }

	template <class T>
	class AlgebraCanThrow : public EitherSummableCanThrow<T> {
	public:
		AlgebraCanThrow(const T& val) : EitherSummableCanThrow(val) {}
	};


	template <class T>
	class PrecisionNumberCanThrow : public EitherComparable<T>, public Comparable,
		public AlgebraCanThrow<T> {
	public:
		explicit
			PrecisionNumberCanThrow(const T& _number = T(), const T& _precision = T())
			: AlgebraCanThrow(_number), epsilon(_precision) {}

		bool operator<(const T& val) const { return (get() < val - epsilon); }
		bool operator<=(const T& val) const { return (get() <= val + epsilon); }
		bool operator==(const T& val) const {
			return (get() >= val - epsilon) && (get() <= val + epsilon);
		}

		bool operator<(const Comparable& obj) const { return (*this) < dynamic_cast<const PrecisionNumberCanThrow&>(obj).get(); }
		bool operator<=(const Comparable& obj) const { return (*this) <= dynamic_cast<const PrecisionNumberCanThrow&>(obj).get(); }
		bool operator==(const Comparable& obj) const { return ((*this) == dynamic_cast<const PrecisionNumberCanThrow&>(obj).get()); }

		PrecisionNumber const & operator= (T const & val) { ElemableCanThrow<T>::operator=(val); return *this; }

		operator T() noexcept { return get(); }

		friend std::ostream& operator<< (std::ostream& o, PrecisionNumberCanThrow const & number);

	private:
		T epsilon;		//our precision
	};

	template <class T>
	inline std::ostream& operator<< (std::ostream& o, PrecisionNumberCanThrow<T> const & number) { return o << number.get(); }

	typedef PrecisionNumberCanThrow<double> AccuracyDoubleCanThrow;

}



//template <double _precision>
//using ConstAccuracyDouble = AccuracyFixedNumber<double, _precision>;

#endif	//ACCURACY_NUMBER_CAN_THROW_H
