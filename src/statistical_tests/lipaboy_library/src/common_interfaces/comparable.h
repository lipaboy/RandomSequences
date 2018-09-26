#ifndef COMPARABLE_H
#define COMPARABLE_H

//Lipa std
namespace LipaboyLib {

	//EitherComparable with other object types
	class Comparable {
	public:
		virtual bool operator< (const Comparable& other) const = 0;
		virtual bool operator<= (const Comparable& other) const {
			return ((*this) < other) || ((*this) == other);
		}
		virtual bool operator== (const Comparable& other) const = 0;

		bool operator> (const Comparable& other) const { return !((*this) <= other); }
		bool operator>= (const Comparable& other) const { return !((*this) < other); }
		bool operator!= (const Comparable& other) const { return !((*this) == other); }
	};

}

#endif //COMPARABLE_H
#pragma once
