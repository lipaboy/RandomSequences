#ifndef EITHER_COMPARABLE_H
#define EITHER_COMPARABLE_H

//Lipa std
namespace LipaboyLib {


	/*------------Too a lot of code production (but more flexible)------------*/

	//TODO: swap (*this) and other by places (more beautifully)

	//EitherComparable with other object types
	template <class T>
	class EitherComparable {
	public:
		virtual bool operator< (const T& other) const = 0;
		virtual bool operator<= (const T& other) const {
			return ((*this) < other) || ((*this) == other);
		}
		virtual bool operator== (const T& other) const = 0;

		bool operator> (const T& other) const { return !((*this) <= other); }
		bool operator>= (const T& other) const { return !((*this) < other); }
		bool operator!= (const T& other) const { return !((*this) == other); }
	};

	template <class T>
	bool operator< (const T& other, const EitherComparable<T>& obj) { return (obj > other); }
	template <class T>
	bool operator<= (const T& other, const EitherComparable<T>& obj) { return (obj >= other); }
	template <class T>
	bool operator== (const T& other, const EitherComparable<T>& obj) { return (obj == other); }
	template <class T>
	bool operator> (const T& other, const EitherComparable<T>& obj) { return (obj < other); }
	template <class T>
	bool operator>= (const T& other, const EitherComparable<T>& obj) { return (obj <= other); }
	template <class T>
	bool operator!= (const T& other, const EitherComparable<T>& obj) { return (obj != other); }

}

#endif //EITHER_COMPARABLE_H
